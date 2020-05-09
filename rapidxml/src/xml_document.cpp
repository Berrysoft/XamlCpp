#include <fstream>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>
#include <sstream>
#include <vector>

using namespace std;
using std::filesystem::path;

namespace rapidxml
{
    class xml_namespace_processor
    {
    private:
        using xmlns_attributes_t = vector<pmr::list<xml_attribute>::iterator>;
        xmlns_attributes_t m_namespace_prefixes;

    public:
        class scope
        {
        private:
            xml_namespace_processor& m_processor;
            size_t const m_stack_position;
            optional<pmr::list<xml_attribute>::iterator> m_default_namespace;

        public:
            scope(xml_namespace_processor& processor)
                : m_processor(processor), m_stack_position(processor.m_namespace_prefixes.size()), m_default_namespace()
            {
            }

            scope(scope const& parent_scope)
                : m_processor(parent_scope.m_processor), m_stack_position(m_processor.m_namespace_prefixes.size()), m_default_namespace(parent_scope.m_default_namespace)
            {
            }

            ~scope()
            {
                m_processor.m_namespace_prefixes.resize(m_stack_position);
            }

            void process_element(xml_node& element)
            {
                optional<pmr::list<xml_attribute>::iterator> pfirst_prefixed_attribute{};
                for (auto attr = element.attributes().begin(); attr != element.attributes().end(); ++attr)
                {
                    switch (attr->prefix().size())
                    {
                    case 0:
                        if (attr->name() == xmlns_namespace::prefix)
                        {
                            attr->namespace_uri(xmlns_namespace::uri);
                            set_default_namespace(attr);
                        }
                        continue;
                    case xml_namespace::prefix.size():
                        if (attr->prefix() == xml_namespace::prefix)
                        {
                            attr->namespace_uri(xml_namespace::uri);
                            continue;
                        }
                        break;
                    case xmlns_namespace::prefix.size():
                        if (attr->prefix() == xmlns_namespace::prefix)
                        {
                            attr->namespace_uri(xmlns_namespace::uri);
                            add_namespace_prefix(attr);
                            continue;
                        }
                        break;
                    } // switch
                    if (!pfirst_prefixed_attribute)
                        pfirst_prefixed_attribute = attr;
                } // for
                if (element.prefix().empty())
                    set_element_default_namespace_uri(element);
                else
                    set_node_namespace_uri_by_prefix(element);

                if (pfirst_prefixed_attribute)
                {
                    auto& first_prefixed_attribute = *pfirst_prefixed_attribute;
                    for (; first_prefixed_attribute != element.attributes().end(); ++first_prefixed_attribute)
                        if (first_prefixed_attribute->prefix().size() > 0 && first_prefixed_attribute->namespace_uri().empty())
                            set_node_namespace_uri_by_prefix(*first_prefixed_attribute);
                }
            }

            void set_default_namespace(pmr::list<xml_attribute>::iterator ns_attr)
            {
                m_default_namespace = ns_attr;
            }

            void add_namespace_prefix(pmr::list<xml_attribute>::iterator ns_attr)
            {
                m_processor.m_namespace_prefixes.push_back(ns_attr);
            }

            void set_element_default_namespace_uri(xml_node& element) const
            {
                if (m_default_namespace)
                    element.namespace_uri((*m_default_namespace)->value());
            }

            void set_node_namespace_uri_by_prefix(xml_base& node) const
            {
                string_view prefix = node.prefix();
                for (typename xml_namespace_processor::xmlns_attributes_t::const_reverse_iterator
                         it = m_processor.m_namespace_prefixes.rbegin();
                     it != m_processor.m_namespace_prefixes.rend(); ++it)
                    if ((*it)->local_name() == prefix)
                    {
                        node.namespace_uri((*it)->value());
                        return;
                    }
                throw parse_error("No namespace definition found", 0, 0, 0);
            }
        };
    };

    void xml_document::load_string(string_view str, parse_flag flags)
    {
        m_buffer.assign(str.begin(), str.end());
        parse(m_buffer.data(), flags);
    }

    void xml_document::load_stream(istream& stream, parse_flag flags)
    {
        m_buffer.assign(istreambuf_iterator<char>(stream), istreambuf_iterator<char>{});
        parse(m_buffer.data(), flags);
    }

    struct file_iterator
    {
        FILE* m_stream;
        char m_current;

        using iterator_category = forward_iterator_tag;
        using value_type = char const;
        using difference_type = intptr_t;
        using pointer = char const*;
        using reference = char const&;

        void move_next() noexcept
        {
            m_current = (char)fgetc(m_stream);
            if (m_current == EOF) m_stream = nullptr;
        }

        constexpr file_iterator() noexcept : m_stream(nullptr), m_current() {}
        file_iterator(FILE* stream) noexcept : m_stream(stream) { move_next(); }

        file_iterator& operator++() noexcept
        {
            move_next();
            return *this;
        }

        file_iterator operator++(int) noexcept
        {
            file_iterator result = *this;
            operator++();
            return result;
        }

        constexpr reference operator*() const noexcept { return m_current; }
        constexpr pointer operator->() const noexcept { return &m_current; }
    };

    constexpr bool operator==(file_iterator const& lhs, file_iterator const& rhs) noexcept { return (!lhs.m_stream) && (!rhs.m_stream); }
    constexpr bool operator!=(file_iterator const& lhs, file_iterator const& rhs) noexcept { return !(lhs == rhs); }

    void xml_document::load_stream(FILE* stream, parse_flag flags)
    {
        m_buffer.assign(file_iterator{ stream }, file_iterator{});
        parse(m_buffer.data(), flags);
    }

    ///////////////////////////////////////////////////////////////////////
    // Internal character utility functions

    // Detect whitespace character
    struct whitespace_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
        }
    };

    // Detect node name character
    struct node_name_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return ch && !whitespace_pred::test(ch) && ch != '/' && ch != '>' && ch != '?';
        }
    };

    // Detect node name character without ':' (NCName) - namespace prefix or local name
    struct node_ncname_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return node_name_pred::test(ch) && ch != ':';
        }
    };

    // Detect attribute name character
    struct attribute_name_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return node_name_pred::test(ch) && ch != '<' && ch != '!' && ch != '=';
        }
    };

    // Detect attribute name character without ':' (NCName) - namespace prefix or local name
    struct attribute_ncname_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return attribute_name_pred::test(ch) && ch != ':';
        }
    };

    // Detectbuffercharacter (PCDATA)
    struct text_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return ch && ch != '<';
        }
    };

    // Detectbuffercharacter (PCDATA) that does not require processing
    struct text_pure_no_ws_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return text_pred::test(ch) && ch != '&';
        }
    };

    // Detectbuffercharacter (PCDATA) that does not require processing
    struct text_pure_with_ws_pred
    {
        static constexpr bool test(char ch) noexcept
        {
            return text_pure_no_ws_pred::test(ch) && whitespace_pred::test(ch);
        }
    };

    // Detect attribute value character
    template <char Quote>
    struct attribute_value_pred
    {
        static_assert(Quote == '\"' || Quote == '\'');

        static constexpr bool test(char ch) noexcept
        {
            return ch && ch != Quote;
        }
    };

    // Detect attribute value character
    template <char Quote>
    struct attribute_value_pure_pred
    {
        static_assert(Quote == '\"' || Quote == '\'');

        static constexpr bool test(char ch) noexcept
        {
            return attribute_value_pred<Quote>::test(ch) && ch != '&';
        }
    };

    struct parse_buffer
    {
        char* const begin;
        char* current;

        constexpr char& operator[](intptr_t index) noexcept { return current[index]; }
        constexpr char const& operator[](intptr_t index) const noexcept { return current[index]; }

        constexpr parse_buffer& operator++()
        {
            current++;
            return *this;
        }

        constexpr parse_buffer operator++(int)
        {
            parse_buffer result = *this;
            operator++();
            return result;
        }

        constexpr parse_buffer& operator+=(intptr_t offset)
        {
            current += offset;
            return *this;
        }

        constexpr operator size_t() const noexcept { return current - begin; }

        constexpr tuple<size_t, size_t> get_row_col() const noexcept
        {
            size_t row = 0, col = current - begin;
            size_t line = 0;
            char* tmp = begin;
            while (tmp < current)
            {
                line++;
                if (*tmp == '\n')
                {
                    row++;
                    col -= line;
                    line = 0;
                }
            }
            return make_tuple(row, col);
        }

        parse_error construct_error(char const* message) const noexcept
        {
            auto [row, col] = get_row_col();
            return parse_error(message, current - begin, col, row);
        }
    };

    // Skip characters until predicate evaluates to true
    template <class StopPred>
    static constexpr void skip(parse_buffer& buffer) noexcept
    {
        char* tmp = buffer.current;
        while (StopPred::test(*tmp))
            ++tmp;
        buffer.current = tmp;
    }

    // Insert coded character, using UTF8
    static constexpr void insert_coded_character(parse_buffer& buffer, int32_t code)
    {
        // Insert UTF8 sequence
        if (code < 0x80) // 1 byte sequence
        {
            buffer[0] = static_cast<char>(code);
            buffer += 1;
        }
        else if (code < 0x800) // 2 byte sequence
        {
            buffer[1] = static_cast<char>((code | 0x80) & 0xBF);
            code >>= 6;
            buffer[0] = static_cast<char>(code | 0xC0);
            buffer += 2;
        }
        else if (code < 0x10000) // 3 byte sequence
        {
            buffer[2] = static_cast<char>((code | 0x80) & 0xBF);
            code >>= 6;
            buffer[1] = static_cast<char>((code | 0x80) & 0xBF);
            code >>= 6;
            buffer[0] = static_cast<char>(code | 0xE0);
            buffer += 3;
        }
        else if (code < 0x110000) // 4 byte sequence
        {
            buffer[3] = static_cast<char>((code | 0x80) & 0xBF);
            code >>= 6;
            buffer[2] = static_cast<char>((code | 0x80) & 0xBF);
            code >>= 6;
            buffer[1] = static_cast<char>((code | 0x80) & 0xBF);
            code >>= 6;
            buffer[0] = static_cast<char>(code | 0xF0);
            buffer += 4;
        }
        else // Invalid, only codes up to 0x10FFFF are allowed in Unicode
        {
            throw buffer.construct_error("invalid numeric character entity");
        }
    }

    // Skip characters until predicate evaluates to true while doing the following:
    // - replacing XML character entity references with proper characters (&apos; &amp; &quot; &lt; &gt; &#...;)
    // - condensing whitespace sequences to single space character
    template <class StopPred, class StopPredPure>
    static char* skip_and_expand_character_refs(parse_buffer& buffer, parse_flag flags)
    {
        // Use simple skip until first modification is detected
        skip<StopPredPure>(buffer);

        // Use translation skip
        parse_buffer src = buffer;
        parse_buffer dest = src;
        while (StopPred::test(src[0]))
        {
            // Test if replacement is needed
            if (src[0] == '&')
            {
                switch (src[1])
                {

                // &amp; &apos;
                case 'a':
                    if (src[2] == 'm' && src[3] == 'p' && src[4] == ';')
                    {
                        dest[0] = '&';
                        ++dest;
                        src += 5;
                        continue;
                    }
                    if (src[2] == 'p' && src[3] == 'o' && src[4] == 's' && src[5] == ';')
                    {
                        dest[0] = '\'';
                        ++dest;
                        src += 6;
                        continue;
                    }
                    break;

                // &quot;
                case 'q':
                    if (src[2] == 'u' && src[3] == 'o' && src[4] == 't' && src[5] == ';')
                    {
                        dest[0] = '"';
                        ++dest;
                        src += 6;
                        continue;
                    }
                    break;

                // &gt;
                case 'g':
                    if (src[2] == 't' && src[3] == ';')
                    {
                        dest[0] = '>';
                        ++dest;
                        src += 4;
                        continue;
                    }
                    break;

                // &lt;
                case 'l':
                    if (src[2] == 't' && src[3] == ';')
                    {
                        dest[0] = '<';
                        ++dest;
                        src += 4;
                        continue;
                    }
                    break;

                // &#...; - assumes ASCII
                case '#':
                    if (src[2] == 'x')
                    {
                        int32_t code = 0;
                        src += 3; // Skip &#x
                        while (1)
                        {
                            if (!isxdigit(src[0]))
                                break;
                            int32_t digit = isdigit(src[0]) ? ((src[0]) - '0') : (isupper(src[0]) ? ((src[0]) - 'A' + 10) : ((src[0]) - 'a' + 10));
                            code = code * 16 + digit;
                            ++src;
                        }
                        insert_coded_character(dest, code); // Put character in output
                    }
                    else
                    {
                        int32_t code = 0;
                        src += 2; // Skip &#
                        while (1)
                        {
                            if (!isdigit(src[0]))
                                break;
                            int32_t digit = ((src[0]) - '0');
                            code = code * 10 + digit;
                            ++src;
                        }
                        insert_coded_character(dest, code); // Put character in output
                    }
                    if (src[0] == ';')
                        ++src;
                    else
                        throw src.construct_error("expected ;");
                    continue;

                // Something else
                default:
                    // Ignore, just copy '&' verbatim
                    break;
                }
            }

            // If whitespace condensing is enabled
            if ((int)(flags & parse_flag::normalize_whitespace))
            {
                // Test if condensing is needed
                if (whitespace_pred::test(src[0]))
                {
                    dest[0] = ' ';
                    ++dest; // Put single space in dest
                    ++src; // Skip first whitespace char
                    // Skip remaining whitespace chars
                    while (whitespace_pred::test(src[0]))
                        ++src;
                    continue;
                }
            }

            // No replacement, only copy character
            (dest++)[0] = (src++)[0];
        }

        // Return new end
        buffer.current = src.current;
        return dest.current;
    }

    // Parse BOM, if any
    static void parse_bom(parse_buffer& buffer)
    {
        // UTF-8?
        if (static_cast<unsigned char>(buffer[0]) == 0xEF &&
            static_cast<unsigned char>(buffer[1]) == 0xBB &&
            static_cast<unsigned char>(buffer[2]) == 0xBF)
        {
            buffer += 3; // Skup utf-8 bom
        }
    }

    struct parse_context
    {
        parse_flag flags;
        pmr::polymorphic_allocator<xml_node> const& node_allocator;
        pmr::polymorphic_allocator<xml_attribute> const& attr_allocator;
    };

    // Parse XML declaration (<?xml...)
    static std::optional<xml_node> parse_xml_declaration(parse_buffer& buffer, parse_context const& context);

    // Parse XML comment (<!--...)
    static std::optional<xml_node> parse_comment(parse_buffer& buffer, parse_context const& context);

    // Parse DOCTYPE
    static std::optional<xml_node> parse_doctype(parse_buffer& buffer, parse_context const& context);

    // Parse PI
    static std::optional<xml_node> parse_pi(parse_buffer& buffer, parse_context const& context);

    // Parse and append data
    // Return character that ends data.
    // This is necessary because this character might have been overwritten by a terminating 0
    static char parse_and_append_data(xml_node& node, parse_buffer& buffer, char* contents_start, parse_context const& context);

    // Parse CDATA
    static std::optional<xml_node> parse_cdata(parse_buffer& buffer, parse_context const& context);

    // Parse element node
    static std::optional<xml_node> parse_element(parse_buffer& buffer, typename xml_namespace_processor::scope namespace_scope, parse_context const& context);

    // Determine node type, and parse it
    static std::optional<xml_node> parse_node(parse_buffer& buffer, typename xml_namespace_processor::scope const& namespace_scope, parse_context const& context);

    // Parse contents of the node - children, data etc.
    static void parse_node_contents(parse_buffer& buffer, xml_node& node, typename xml_namespace_processor::scope const& namespace_scope, parse_context const& context);

    // Parse XML attributes of the node
    static void parse_node_attributes(parse_buffer& buffer, xml_node& node, parse_context const& context);

    void xml_document::parse(char* text, parse_flag flags)
    {
        if (text)
        {
            parse_buffer buffer{ text, text };

            xml_namespace_processor namespace_processor;
            // Creating topmost namespace scope that actually won't be used
            typename xml_namespace_processor::scope const namespace_scope(namespace_processor);

            // Parse BOM, if any
            parse_bom(buffer);

            // Parse children
            while (1)
            {
                // Skip whitespace before node
                skip<whitespace_pred>(buffer);
                if (!buffer[0])
                    break;

                // Parse and append new child
                if (buffer[0] == '<')
                {
                    ++buffer; // Skip '<'
                    if (auto node = parse_node(buffer, namespace_scope, { flags, m_node_allocator, m_attribute_allocator }))
                        m_root_node.nodes().emplace_back(move(*node));
                }
                else
                    throw buffer.construct_error("expected <");
            }
        }
    }

    optional<xml_node> parse_xml_declaration(parse_buffer& buffer, parse_context const& context)
    {
        // If parsing of declaration is disabled
        if (!(int)(context.flags & parse_flag::declaration_node))
        {
            // Skip until end of declaration
            while (buffer[0] != '?' || buffer[1] != '>')
            {
                if (!buffer[0])
                    throw buffer.construct_error("unexpected end of data");
                ++buffer;
            }
            buffer += 2; // Skip '?>'
            return nullopt;
        }

        // Create declaration
        xml_node declaration{ node_type::declaration, context.node_allocator, context.attr_allocator };

        // Skip whitespace before attributes or ?>
        skip<whitespace_pred>(buffer);

        // Parse declaration attributes
        parse_node_attributes(buffer, declaration, context);

        // Skip ?>
        if (buffer[0] != '?' || buffer[1] != '>')
            throw buffer.construct_error("expected ?>");
        buffer += 2;

        return make_optional(move(declaration));
    }

    optional<xml_node> parse_comment(parse_buffer& buffer, parse_context const& context)
    {
        // If parsing of comments is disabled
        if (!(int)(context.flags & parse_flag::comment_nodes))
        {
            // Skip until end of comment
            while (buffer[0] != '-' || buffer[1] != '-' || buffer[2] != '>')
            {
                if (!buffer[0])
                    throw buffer.construct_error("unexpected end of data");
                ++buffer;
            }
            buffer += 3; // Skip '-->'
            return nullopt; // Do not produce comment node
        }

        // Remember value start
        char* value = buffer.current;

        // Skip until end of comment
        while (buffer[0] != '-' || buffer[1] != '-' || buffer[2] != '>')
        {
            if (!buffer[0])
                throw buffer.construct_error("unexpected end of data");
            ++buffer;
        }

        // Create comment node
        xml_node comment{ node_type::comment, context.node_allocator, context.attr_allocator };
        comment.value(string_view(value, buffer.current - value));

        buffer += 3; // Skip '-->'
        return make_optional(move(comment));
    }

    optional<xml_node> parse_doctype(parse_buffer& buffer, parse_context const& context)
    {
        // Remember value start
        char* value = buffer.current;

        // Skip to >
        while (buffer[0] != '>')
        {
            // Determine character type
            switch (buffer[0])
            {

            // If '[' encountered, scan for matching ending ']' using naive algorithm with depth
            // This works for all W3C test files except for 2 most wicked
            case '[':
            {
                ++buffer; // Skip '['
                int depth = 1;
                while (depth > 0)
                {
                    switch (buffer[0])
                    {
                    case '[':
                        ++depth;
                        break;
                    case ']':
                        --depth;
                        break;
                    case 0:
                        throw buffer.construct_error("unexpected end of data");
                    }
                    ++buffer;
                }
                break;
            }

            // Error on end of text
            case '\0':
                throw buffer.construct_error("unexpected end of data");

            // Other character, skip it
            default:
                ++buffer;
            }
        }

        // If DOCTYPE nodes enabled
        if ((int)(context.flags & parse_flag::doctype_node))
        {
            // Create a new doctype node
            xml_node doctype{ node_type::doctype, context.node_allocator, context.attr_allocator };
            doctype.value(string_view(value, buffer.current - value));

            buffer += 1; // skip '>'
            return make_optional(move(doctype));
        }
        else
        {
            buffer += 1; // skip '>'
            return nullopt;
        }
    }

    optional<xml_node> parse_pi(parse_buffer& buffer, parse_context const& context)
    {
        // If creation of PI nodes is enabled
        if ((int)(context.flags & parse_flag::pi_nodes))
        {
            // Create pi node
            xml_node pi{ node_type::pi, context.node_allocator, context.attr_allocator };

            // Extract PI target name
            char* name = buffer.current;
            skip<node_name_pred>(buffer);
            if (buffer.current == name)
                throw buffer.construct_error("expected PI target");
            pi.name(string_view(name, buffer.current - name));

            // Skip whitespace between pi target and pi
            skip<whitespace_pred>(buffer);

            // Remember start of pi
            char* value = buffer.current;

            // Skip to '?>'
            while (buffer[0] != '?' || buffer[1] != '>')
            {
                if (buffer[0] == '\0')
                    throw buffer.construct_error("unexpected end of data");
                ++buffer;
            }

            // Set pi value (verbatim, no entity expansion or whitespace normalization)
            pi.value(string_view(value, buffer.current - value));

            buffer += 2; // Skip '?>'
            return make_optional(move(pi));
        }
        else
        {
            // Skip to '?>'
            while (buffer[0] != '?' || buffer[1] != '>')
            {
                if (buffer[0] == '\0')
                    throw buffer.construct_error("unexpected end of data");
                ++buffer;
            }
            buffer += 2; // Skip '?>'
            return nullopt;
        }
    }

    char parse_and_append_data(xml_node& node, parse_buffer& buffer, char* contents_start, parse_context const& context)
    {
        // Backup to contents start if whitespace trimming is disabled
        if (!(int)(context.flags & parse_flag::trim_whitespace))
            buffer.current = contents_start;

        // Skip until end of data
        char *value = buffer.current, *end;
        if ((int)(context.flags & parse_flag::normalize_whitespace))
            end = skip_and_expand_character_refs<text_pred, text_pure_with_ws_pred>(buffer, context.flags);
        else
            end = skip_and_expand_character_refs<text_pred, text_pure_no_ws_pred>(buffer, context.flags);

        // Trim trailing whitespace if flag is set; leading was already trimmed by whitespace skip after >
        if ((int)(context.flags & parse_flag::trim_whitespace))
        {
            if ((int)(context.flags & parse_flag::normalize_whitespace))
            {
                // Whitespace is already condensed to single space characters by skipping function, so just trim 1 char off the end
                if (*(end - 1) == ' ')
                    --end;
            }
            else
            {
                // Backup until non-whitespace character is found
                while (whitespace_pred::test(*(end - 1)))
                    --end;
            }
        }

        // If characters are still left between end and value (this test is only necessary if normalization is enabled)
        // Create new data node
        if (!(int)(context.flags & parse_flag::no_data_nodes))
        {
            xml_node data{ node_type::data, context.node_allocator, context.attr_allocator };
            data.value(string_view(value, end - value));
            node.nodes().emplace_back(move(data));
        }

        // Add data to parent node if no data exists yet
        if (!(int)(context.flags & parse_flag::no_element_values))
            if (node.value().empty())
                node.value(string_view(value, end - value));

        // Return character that ends data
        return buffer[0];
    }

    optional<xml_node> parse_cdata(parse_buffer& buffer, parse_context const& context)
    {
        // If CDATA is disabled
        if ((int)(context.flags & parse_flag::no_data_nodes))
        {
            // Skip until end of cdata
            while (buffer[0] != ']' || buffer[1] != ']' || buffer[2] != '>')
            {
                if (!buffer[0])
                    throw buffer.construct_error("unexpected end of data");
                ++buffer;
            }
            buffer += 3; // Skip ]]>
            return nullopt; // Do not produce CDATA node
        }

        // Skip until end of cdata
        char* value = buffer.current;
        while (buffer[0] != ']' || buffer[1] != ']' || buffer[2] != '>')
        {
            if (!buffer[0])
                throw buffer.construct_error("unexpected end of data");
            ++buffer;
        }

        // Create new cdata node
        xml_node cdata{ node_type::cdata, context.node_allocator, context.attr_allocator };
        cdata.value(string_view(value, buffer.current - value));

        buffer += 3; // Skip ]]>
        return make_optional(move(cdata));
    }

    optional<xml_node> parse_element(parse_buffer& buffer, typename xml_namespace_processor::scope namespace_scope, parse_context const& context)
    {
        // Create element node
        xml_node element{ node_type::element, context.node_allocator, context.attr_allocator };

        // Extract element name
        char* name = buffer.current;
        skip<node_ncname_pred>(buffer);
        if (buffer.current == name)
            throw buffer.construct_error("expected element name");
        if (buffer[0] == ':')
        {
            // Namespace prefix found
            ++buffer;
            char* local_name = buffer.current;
            skip<node_ncname_pred>(buffer);
            if (buffer[0] == ':')
                throw buffer.construct_error("second colon in element name");
            if (buffer.current == local_name)
                throw buffer.construct_error("expected local part of element name");
            element.qname(string_view(name, buffer.current - name), local_name - name);
        }
        else
            element.qname(string_view(name, buffer.current - name));

        // Skip whitespace between element name and attributes or >
        skip<whitespace_pred>(buffer);

        // Parse attributes, if any
        parse_node_attributes(buffer, element, context);

        // Setting attributes and element own namespace_uri, adding declared
        // namespace prefixes and probably setting default namespace
        namespace_scope.process_element(element);

        // Determine ending type
        if (buffer[0] == '>')
        {
            ++buffer;
            parse_node_contents(buffer, element, namespace_scope, context);
        }
        else if (buffer[0] == '/')
        {
            ++buffer;
            if (buffer[0] != '>')
                throw buffer.construct_error("expected >");
            ++buffer;
        }
        else
            throw buffer.construct_error("expected >");

        // Return parsed element
        return make_optional(move(element));
    }

    optional<xml_node> parse_node(parse_buffer& buffer, typename xml_namespace_processor::scope const& namespace_scope, parse_context const& context)
    {
        // Parse proper node type
        switch (buffer[0])
        {

        // <...
        default:
            // Parse and append element node
            return parse_element(buffer, namespace_scope, context);

        // <?...
        case '?':
            ++buffer; // Skip ?
            if ((buffer[0] == 'x' || buffer[0] == 'X') &&
                (buffer[1] == 'm' || buffer[1] == 'M') &&
                (buffer[2] == 'l' || buffer[2] == 'L') &&
                whitespace_pred::test(buffer[3]))
            {
                // '<?xml ' - xml declaration
                buffer += 4; // Skip 'xml '
                return parse_xml_declaration(buffer, context);
            }
            else
            {
                // Parse PI
                return parse_pi(buffer, context);
            }

        // <!...
        case '!':

            // Parse proper subset of <! node
            switch (buffer[1])
            {

            // <!-
            case '-':
                if (buffer[2] == '-')
                {
                    // '<!--' - xml comment
                    buffer += 3; // Skip '!--'
                    return parse_comment(buffer, context);
                }
                break;

            // <![
            case '[':
                if (buffer[2] == 'C' && buffer[3] == 'D' && buffer[4] == 'A' &&
                    buffer[5] == 'T' && buffer[6] == 'A' && buffer[7] == '[')
                {
                    // '<![CDATA[' - cdata
                    buffer += 8; // Skip '![CDATA['
                    return parse_cdata(buffer, context);
                }
                break;

            // <!D
            case 'D':
                if (buffer[2] == 'O' && buffer[3] == 'C' && buffer[4] == 'T' &&
                    buffer[5] == 'Y' && buffer[6] == 'P' && buffer[7] == 'E' &&
                    whitespace_pred::test(buffer[8]))
                {
                    // '<!DOCTYPE ' - doctype
                    buffer += 9; // skip '!DOCTYPE '
                    return parse_doctype(buffer, context);
                }

            } // switch

            // Attempt to skip other, unrecognized node types starting with <!
            ++buffer; // Skip !
            while (buffer[0] != '>')
            {
                if (buffer[0] == 0)
                    throw buffer.construct_error("unexpected end of data");
                ++buffer;
            }
            ++buffer; // Skip '>'
            return nullopt; // No node recognized
        }
    }

    void parse_node_contents(parse_buffer& buffer, xml_node& node, typename xml_namespace_processor::scope const& namespace_scope, parse_context const& context)
    {
        // For all children and text
        while (1)
        {
            // Skip whitespace between > and node contents
            char* contents_start = buffer.current; // Store start of node contents before whitespace is skipped
            skip<whitespace_pred>(buffer);
            char next_char = buffer[0];

        // After data nodes, instead of continuing the loop, control jumps here.
        // This is because zero termination inside parse_and_append_data() function
        // would wreak havoc with the above code.
        // Also, skipping whitespace after data nodes is unnecessary.
        after_data_node:

            // Determine what comes next: node closing, child node, data node, or 0?
            switch (next_char)
            {

            // Node closing or child node
            case '<':
                if (buffer[1] == '/')
                {
                    // Node closing
                    buffer += 2; // Skip '</'
                    if ((int)(context.flags & parse_flag::validate_closing_tags))
                    {
                        // Skip and validate closing tag name
                        char* closing_name = buffer.current;
                        skip<node_name_pred>(buffer);
                        if (node.name() != string_view(closing_name, buffer.current - closing_name))
                            throw buffer.construct_error("invalid closing tag name");
                    }
                    else
                    {
                        // No validation, just skip name
                        skip<node_name_pred>(buffer);
                    }
                    // Skip remaining whitespace after node name
                    skip<whitespace_pred>(buffer);
                    if (buffer[0] != '>')
                        throw buffer.construct_error("expected >");
                    ++buffer; // Skip '>'
                    return; // Node closed, finished parsing contents
                }
                else
                {
                    // charild node
                    ++buffer; // Skip '<'
                    if (auto child = parse_node(buffer, namespace_scope, context))
                        node.nodes().emplace_back(move(*child));
                }
                break;

            // End of data - error
            case '\0':
                throw buffer.construct_error("unexpected end of data");

            // Data node
            default:
                next_char = parse_and_append_data(node, buffer, contents_start, context);
                goto after_data_node; // Bypass regular processing after data nodes
            }
        }
    }

    void parse_node_attributes(parse_buffer& buffer, xml_node& node, parse_context const& context)
    {
        // For all attributes
        while (attribute_ncname_pred::test(buffer[0]))
        {
            // Extract attribute name
            parse_buffer name = buffer;
            ++buffer; // Skip first character of attribute name
            skip<attribute_ncname_pred>(buffer);
            if (buffer.current == name.current)
                throw name.construct_error("expected attribute name");
            // Create new attribute
            xml_attribute attribute{};
            if (buffer[0] == ':')
            {
                // Namespace prefix found
                ++buffer;
                parse_buffer local_name = buffer;
                skip<attribute_ncname_pred>(buffer);
                if (buffer.current == local_name.current)
                    throw local_name.construct_error("expected local part of attribute name");
                attribute.qname(string_view(name.current, buffer.current - name.current), local_name.current - name.current);
            }
            else
                attribute.qname(string_view(name.current, buffer.current - name.current));

            // Skip whitespace after attribute name
            skip<whitespace_pred>(buffer);

            // Skip =
            if (buffer[0] != '=')
                throw buffer.construct_error("expected =");
            ++buffer;

            // Skip whitespace after =
            skip<whitespace_pred>(buffer);

            // Skip quote and remember if it was ' or "
            char quote = buffer[0];
            if (quote != '\'' && quote != '"')
                throw buffer.construct_error("expected ' or \"");
            ++buffer;

            // Extract attribute value and expand char refs in it
            char *value = buffer.current, *end;
            parse_flag AttFlags{ context.flags & ~parse_flag::normalize_whitespace }; // No whitespace normalization in attributes
            if (quote == '\'')
                end = skip_and_expand_character_refs<attribute_value_pred<'\''>, attribute_value_pure_pred<'\''>>(buffer, AttFlags);
            else
                end = skip_and_expand_character_refs<attribute_value_pred<'"'>, attribute_value_pure_pred<'"'>>(buffer, AttFlags);

            // Set attribute value
            attribute.value(string_view(value, end - value));

            // Make sure that end quote is present
            if (buffer[0] != quote)
                throw buffer.construct_error("expected ' or \"");
            ++buffer; // Skip quote

            // Skip whitespace after attribute value
            skip<whitespace_pred>(buffer);

            node.attributes().emplace_back(move(attribute));
        }
    }
} // namespace rapidxml

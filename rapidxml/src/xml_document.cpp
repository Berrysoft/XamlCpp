#include <cassert>
#include <cctype>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>
#include <vector>

using namespace std;

namespace rapidxml
{
    namespace internal
    {
        void xml_namespace_processor::scope::process_element(xml_node* element)
        {
            xml_attribute* first_prefixed_attribute = 0;
            for (xml_attribute* attr = element->first_attribute(); attr; attr = attr->next_attribute())
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
                if (!first_prefixed_attribute)
                    first_prefixed_attribute = attr;
            } // for
            if (element->prefix().empty())
                set_element_default_namespace_uri(element);
            else
                set_node_namespace_uri_by_prefix(element);

            for (; first_prefixed_attribute; first_prefixed_attribute = first_prefixed_attribute->next_attribute())
                if (first_prefixed_attribute->prefix().size() > 0 && first_prefixed_attribute->namespace_uri().empty())
                    set_node_namespace_uri_by_prefix(first_prefixed_attribute);
        }

        void xml_namespace_processor::scope::set_element_default_namespace_uri(xml_node* element) const
        {
            if (m_default_namespace)
                element->namespace_uri(m_default_namespace->value());
        }

        void xml_namespace_processor::scope::set_node_namespace_uri_by_prefix(xml_base* node) const
        {
            string_view prefix = node->prefix();
            for (typename xml_namespace_processor::xmlns_attributes_t::const_reverse_iterator
                     it = m_processor.m_namespace_prefixes.rbegin();
                 it != m_processor.m_namespace_prefixes.rend(); ++it)
                if ((*it)->local_name() == prefix)
                {
                    node->namespace_uri((*it)->value());
                    return;
                }
            throw parse_error("No namespace definition found", nullptr);
        }
    } // namespace internal

    ///////////////////////////////////////////////////////////////////////
    // Internal character utility functions

    // Detect whitespace character
    struct whitespace_pred
    {
        static constexpr bool test(char ch)
        {
            return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
        }
    };

    // Detect node name character
    struct node_name_pred
    {
        static constexpr bool test(char ch)
        {
            return ch && !whitespace_pred::test(ch) && ch != '/' && ch != '>' && ch != '?';
        }
    };

    // Detect node name character without ':' (NCName) - namespace prefix or local name
    struct node_ncname_pred
    {
        static constexpr bool test(char ch)
        {
            return node_name_pred::test(ch) && ch != ':';
        }
    };

    // Detect attribute name character
    struct attribute_name_pred
    {
        static constexpr bool test(char ch)
        {
            return node_name_pred::test(ch) && ch != '<' && ch != '!' && ch != '=';
        }
    };

    // Detect attribute name character without ':' (NCName) - namespace prefix or local name
    struct attribute_ncname_pred
    {
        static constexpr bool test(char ch)
        {
            return attribute_name_pred::test(ch) && ch != ':';
        }
    };

    // Detect text character (PCDATA)
    struct text_pred
    {
        static constexpr bool test(char ch)
        {
            return ch && ch != '<';
        }
    };

    // Detect text character (PCDATA) that does not require processing
    struct text_pure_no_ws_pred
    {
        static constexpr bool test(char ch)
        {
            return text_pred::test(ch) && ch != '&';
        }
    };

    // Detect text character (PCDATA) that does not require processing
    struct text_pure_with_ws_pred
    {
        static constexpr bool test(char ch)
        {
            return text_pure_no_ws_pred::test(ch) && whitespace_pred::test(ch);
        }
    };

    // Detect attribute value character
    template <char Quote>
    struct attribute_value_pred
    {
        static_assert(Quote == '\"' || Quote == '\'');

        static constexpr bool test(char ch)
        {
            return ch && ch != Quote;
        }
    };

    // Detect attribute value character
    template <char Quote>
    struct attribute_value_pure_pred
    {
        static_assert(Quote == '\"' || Quote == '\'');

        static constexpr bool test(char ch)
        {
            return attribute_value_pred<Quote>::test(ch) && ch != '&';
        }
    };

    // Skip characters until predicate evaluates to true
    template <class StopPred>
    static constexpr void skip(char*& text)
    {
        char* tmp = text;
        while (StopPred::test(*tmp))
            ++tmp;
        text = tmp;
    }

    // Insert coded character, using UTF8 or 8-bit ASCII
    static void insert_coded_character(char*& text, unsigned long code, parse_flag flags)
    {
        // Insert UTF8 sequence
        if (code < 0x80) // 1 byte sequence
        {
            text[0] = static_cast<unsigned char>(code);
            text += 1;
        }
        else if (code < 0x800) // 2 byte sequence
        {
            text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF);
            code >>= 6;
            text[0] = static_cast<unsigned char>(code | 0xC0);
            text += 2;
        }
        else if (code < 0x10000) // 3 byte sequence
        {
            text[2] = static_cast<unsigned char>((code | 0x80) & 0xBF);
            code >>= 6;
            text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF);
            code >>= 6;
            text[0] = static_cast<unsigned char>(code | 0xE0);
            text += 3;
        }
        else if (code < 0x110000) // 4 byte sequence
        {
            text[3] = static_cast<unsigned char>((code | 0x80) & 0xBF);
            code >>= 6;
            text[2] = static_cast<unsigned char>((code | 0x80) & 0xBF);
            code >>= 6;
            text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF);
            code >>= 6;
            text[0] = static_cast<unsigned char>(code | 0xF0);
            text += 4;
        }
        else // Invalid, only codes up to 0x10FFFF are allowed in Unicode
        {
            throw parse_error("invalid numeric character entity", text);
        }
    }

    // Skip characters until predicate evaluates to true while doing the following:
    // - replacing XML character entity references with proper characters (&apos; &amp; &quot; &lt; &gt; &#...;)
    // - condensing whitespace sequences to single space character
    template <class StopPred, class StopPredPure>
    static char* skip_and_expand_character_refs(char*& text, parse_flag flags)
    {
        // Use simple skip until first modification is detected
        skip<StopPredPure>(text);

        // Use translation skip
        char* src = text;
        char* dest = src;
        while (StopPred::test(*src))
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
                        *dest = '&';
                        ++dest;
                        src += 5;
                        continue;
                    }
                    if (src[2] == 'p' && src[3] == 'o' && src[4] == 's' && src[5] == ';')
                    {
                        *dest = char('\'');
                        ++dest;
                        src += 6;
                        continue;
                    }
                    break;

                // &quot;
                case 'q':
                    if (src[2] == 'u' && src[3] == 'o' && src[4] == 't' && src[5] == ';')
                    {
                        *dest = '"';
                        ++dest;
                        src += 6;
                        continue;
                    }
                    break;

                // &gt;
                case 'g':
                    if (src[2] == 't' && src[3] == ';')
                    {
                        *dest = '>';
                        ++dest;
                        src += 4;
                        continue;
                    }
                    break;

                // &lt;
                case 'l':
                    if (src[2] == 't' && src[3] == ';')
                    {
                        *dest = '<';
                        ++dest;
                        src += 4;
                        continue;
                    }
                    break;

                // &#...; - assumes ASCII
                case '#':
                    if (src[2] == 'x')
                    {
                        unsigned long code = 0;
                        src += 3; // Skip &#x
                        while (1)
                        {
                            if (!isxdigit(*src))
                                break;
                            unsigned long digit = isdigit(*src) ? ((*src) - '0') : (isupper(*src) ? ((*src) - 'A' + 10) : ((*src) - 'a' + 10));
                            code = code * 16 + digit;
                            ++src;
                        }
                        insert_coded_character(dest, code, flags); // Put character in output
                    }
                    else
                    {
                        unsigned long code = 0;
                        src += 2; // Skip &#
                        while (1)
                        {
                            if (!isdigit(*src))
                                break;
                            unsigned long digit = ((*src) - '0');
                            code = code * 10 + digit;
                            ++src;
                        }
                        insert_coded_character(dest, code, flags); // Put character in output
                    }
                    if (*src == ';')
                        ++src;
                    else
                        throw parse_error("expected ;", src);
                    continue;

                // Something else
                default:
                    // Ignore, just copy '&' verbatim
                    break;
                }
            }

            // If whitespace condensing is enabled
            if (flags & parse_flag::normalize_whitespace)
            {
                // Test if condensing is needed
                if (whitespace_pred::test(*src))
                {
                    *dest = ' ';
                    ++dest; // Put single space in dest
                    ++src; // Skip first whitespace char
                    // Skip remaining whitespace chars
                    while (whitespace_pred::test(*src))
                        ++src;
                    continue;
                }
            }

            // No replacement, only copy character
            *dest++ = *src++;
        }

        // Return new end
        text = src;
        return dest;
    }

    // Parse BOM, if any
    static void parse_bom(char*& text)
    {
        // UTF-8?
        if (static_cast<unsigned char>(text[0]) == 0xEF &&
            static_cast<unsigned char>(text[1]) == 0xBB &&
            static_cast<unsigned char>(text[2]) == 0xBF)
        {
            text += 3; // Skup utf-8 bom
        }
    }

    void xml_document::parse(char* text, parse_flag flags)
    {
        assert(text);

        // Remove current contents
        this->remove_all_nodes();
        this->remove_all_attributes();

        internal::xml_namespace_processor namespace_processor;
        // Creating topmost namespace scope that actually won't be used
        typename internal::xml_namespace_processor::scope const namespace_scope(namespace_processor);

        // Parse BOM, if any
        parse_bom(text);

        // Parse children
        while (1)
        {
            // Skip whitespace before node
            skip<whitespace_pred>(text);
            if (*text == 0)
                break;

            // Parse and append new child
            if (*text == '<')
            {
                ++text; // Skip '<'
                if (xml_node* node = parse_node(text, namespace_scope, flags))
                    this->append_node(node);
            }
            else
                throw parse_error("expected <", text);
        }
    }

    void xml_document::clear()
    {
        this->remove_all_nodes();
        this->remove_all_attributes();
        pool.clear();
    }

    xml_node* xml_document::parse_xml_declaration(char*& text, parse_flag flags)
    {
        // If parsing of declaration is disabled
        if (!(flags & parse_flag::declaration_node))
        {
            // Skip until end of declaration
            while (text[0] != '?' || text[1] != '>')
            {
                if (!text[0])
                    throw parse_error("unexpected end of data", text);
                ++text;
            }
            text += 2; // Skip '?>'
            return nullptr;
        }

        // Create declaration
        xml_node* declaration = pool.allocate_node(node_type::declaration);

        // Skip whitespace before attributes or ?>
        skip<whitespace_pred>(text);

        // Parse declaration attributes
        parse_node_attributes(text, declaration, flags);

        // Skip ?>
        if (text[0] != '?' || text[1] != '>')
            throw parse_error("expected ?>", text);
        text += 2;

        return declaration;
    }

    xml_node* xml_document::parse_comment(char*& text, parse_flag flags)
    {
        // If parsing of comments is disabled
        if (!(flags & parse_flag::comment_nodes))
        {
            // Skip until end of comment
            while (text[0] != '-' || text[1] != '-' || text[2] != '>')
            {
                if (!text[0])
                    throw parse_error("unexpected end of data", text);
                ++text;
            }
            text += 3; // Skip '-->'
            return nullptr; // Do not produce comment node
        }

        // Remember value start
        char* value = text;

        // Skip until end of comment
        while (text[0] != '-' || text[1] != '-' || text[2] != '>')
        {
            if (!text[0])
                throw parse_error("unexpected end of data", text);
            ++text;
        }

        // Create comment node
        xml_node* comment = pool.allocate_node(node_type::comment);
        comment->value(string_view(value, text - value));

        text += 3; // Skip '-->'
        return comment;
    }

    xml_node* xml_document::parse_doctype(char*& text, parse_flag flags)
    {
        // Remember value start
        char* value = text;

        // Skip to >
        while (*text != '>')
        {
            // Determine character type
            switch (*text)
            {

            // If '[' encountered, scan for matching ending ']' using naive algorithm with depth
            // This works for all W3C test files except for 2 most wicked
            case '[':
            {
                ++text; // Skip '['
                int depth = 1;
                while (depth > 0)
                {
                    switch (*text)
                    {
                    case '[':
                        ++depth;
                        break;
                    case ']':
                        --depth;
                        break;
                    case 0:
                        throw parse_error("unexpected end of data", text);
                    }
                    ++text;
                }
                break;
            }

            // Error on end of text
            case '\0':
                throw parse_error("unexpected end of data", text);

            // Other character, skip it
            default:
                ++text;
            }
        }

        // If DOCTYPE nodes enabled
        if (flags & parse_flag::doctype_node)
        {
            // Create a new doctype node
            xml_node* doctype = pool.allocate_node(node_type::doctype);
            doctype->value(string_view(value, text - value));

            text += 1; // skip '>'
            return doctype;
        }
        else
        {
            text += 1; // skip '>'
            return nullptr;
        }
    }

    xml_node* xml_document::parse_pi(char*& text, parse_flag flags)
    {
        // If creation of PI nodes is enabled
        if (flags & parse_flag::pi_nodes)
        {
            // Create pi node
            xml_node* pi = pool.allocate_node(node_type::pi);

            // Extract PI target name
            char* name = text;
            skip<node_name_pred>(text);
            if (text == name)
                throw parse_error("expected PI target", text);
            pi->name(string_view(name, text - name));

            // Skip whitespace between pi target and pi
            skip<whitespace_pred>(text);

            // Remember start of pi
            char* value = text;

            // Skip to '?>'
            while (text[0] != '?' || text[1] != '>')
            {
                if (*text == '\0')
                    throw parse_error("unexpected end of data", text);
                ++text;
            }

            // Set pi value (verbatim, no entity expansion or whitespace normalization)
            pi->value(string_view(value, text - value));

            text += 2; // Skip '?>'
            return pi;
        }
        else
        {
            // Skip to '?>'
            while (text[0] != '?' || text[1] != '>')
            {
                if (*text == '\0')
                    throw parse_error("unexpected end of data", text);
                ++text;
            }
            text += 2; // Skip '?>'
            return nullptr;
        }
    }

    char xml_document::parse_and_append_data(xml_node* node, char*& text, char* contents_start, parse_flag flags)
    {
        // Backup to contents start if whitespace trimming is disabled
        if (!(flags & parse_flag::trim_whitespace))
            text = contents_start;

        // Skip until end of data
        char *value = text, *end;
        if (flags & parse_flag::normalize_whitespace)
            end = skip_and_expand_character_refs<text_pred, text_pure_with_ws_pred>(text, flags);
        else
            end = skip_and_expand_character_refs<text_pred, text_pure_no_ws_pred>(text, flags);

        // Trim trailing whitespace if flag is set; leading was already trimmed by whitespace skip after >
        if (flags & parse_flag::trim_whitespace)
        {
            if (flags & parse_flag::normalize_whitespace)
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
        if (!(flags & parse_flag::no_data_nodes))
        {
            xml_node* data = pool.allocate_node(node_type::data);
            data->value(string_view(value, end - value));
            node->append_node(data);
        }

        // Add data to parent node if no data exists yet
        if (!(flags & parse_flag::no_element_values))
            if (node->value().empty())
                node->value(string_view(value, end - value));

        // Return character that ends data
        return *text;
    }

    xml_node* xml_document::parse_cdata(char*& text, parse_flag flags)
    {
        // If CDATA is disabled
        if (flags & parse_flag::no_data_nodes)
        {
            // Skip until end of cdata
            while (text[0] != ']' || text[1] != ']' || text[2] != '>')
            {
                if (!text[0])
                    throw parse_error("unexpected end of data", text);
                ++text;
            }
            text += 3; // Skip ]]>
            return nullptr; // Do not produce CDATA node
        }

        // Skip until end of cdata
        char* value = text;
        while (text[0] != ']' || text[1] != ']' || text[2] != '>')
        {
            if (!text[0])
                throw parse_error("unexpected end of data", text);
            ++text;
        }

        // Create new cdata node
        xml_node* cdata = pool.allocate_node(node_type::cdata);
        cdata->value(string_view(value, text - value));

        text += 3; // Skip ]]>
        return cdata;
    }

    xml_node* xml_document::parse_element(char*& text, typename internal::xml_namespace_processor::scope namespace_scope, parse_flag flags)
    {
        // Create element node
        xml_node* element = pool.allocate_node(node_type::element);

        // Extract element name
        char* name = text;
        skip<node_ncname_pred>(text);
        if (text == name)
            throw parse_error("expected element name", text);
        if (*text == ':')
        {
            // Namespace prefix found
            ++text;
            char* local_name = text;
            skip<node_ncname_pred>(text);
            if (*text == ':')
                throw parse_error("second colon in element name", text);
            if (text == local_name)
                throw parse_error("expected local part of element name", text);
            element->qname(string_view(name, text - name), local_name - name);
        }
        else
            element->qname(string_view(name, text - name));

        // Skip whitespace between element name and attributes or >
        skip<whitespace_pred>(text);

        // Parse attributes, if any
        parse_node_attributes(text, element, flags);

        // Setting attributes and element own namespace_uri, adding declared
        // namespace prefixes and probably setting default namespace
        namespace_scope.process_element(element);

        // Determine ending type
        if (*text == '>')
        {
            ++text;
            parse_node_contents(text, element, namespace_scope, flags);
        }
        else if (*text == '/')
        {
            ++text;
            if (*text != '>')
                throw parse_error("expected >", text);
            ++text;
        }
        else
            throw parse_error("expected >", text);

        // Return parsed element
        return element;
    }

    xml_node* xml_document::parse_node(char*& text, typename internal::xml_namespace_processor::scope const& namespace_scope, parse_flag flags)
    {
        // Parse proper node type
        switch (text[0])
        {

        // <...
        default:
            // Parse and append element node
            return parse_element(text, namespace_scope, flags);

        // <?...
        case '?':
            ++text; // Skip ?
            if ((text[0] == 'x' || text[0] == 'X') &&
                (text[1] == 'm' || text[1] == 'M') &&
                (text[2] == 'l' || text[2] == 'L') &&
                whitespace_pred::test(text[3]))
            {
                // '<?xml ' - xml declaration
                text += 4; // Skip 'xml '
                return parse_xml_declaration(text, flags);
            }
            else
            {
                // Parse PI
                return parse_pi(text, flags);
            }

        // <!...
        case '!':

            // Parse proper subset of <! node
            switch (text[1])
            {

            // <!-
            case '-':
                if (text[2] == '-')
                {
                    // '<!--' - xml comment
                    text += 3; // Skip '!--'
                    return parse_comment(text, flags);
                }
                break;

            // <![
            case '[':
                if (text[2] == 'C' && text[3] == 'D' && text[4] == 'A' &&
                    text[5] == 'T' && text[6] == 'A' && text[7] == '[')
                {
                    // '<![CDATA[' - cdata
                    text += 8; // Skip '![CDATA['
                    return parse_cdata(text, flags);
                }
                break;

            // <!D
            case 'D':
                if (text[2] == 'O' && text[3] == 'C' && text[4] == 'T' &&
                    text[5] == 'Y' && text[6] == 'P' && text[7] == 'E' &&
                    whitespace_pred::test(text[8]))
                {
                    // '<!DOCTYPE ' - doctype
                    text += 9; // skip '!DOCTYPE '
                    return parse_doctype(text, flags);
                }

            } // switch

            // Attempt to skip other, unrecognized node types starting with <!
            ++text; // Skip !
            while (*text != '>')
            {
                if (*text == 0)
                    throw parse_error("unexpected end of data", text);
                ++text;
            }
            ++text; // Skip '>'
            return nullptr; // No node recognized
        }
    }

    void xml_document::parse_node_contents(char*& text, xml_node* node, typename internal::xml_namespace_processor::scope const& namespace_scope, parse_flag flags)
    {
        // For all children and text
        while (1)
        {
            // Skip whitespace between > and node contents
            char* contents_start = text; // Store start of node contents before whitespace is skipped
            skip<whitespace_pred>(text);
            char next_char = *text;

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
                if (text[1] == '/')
                {
                    // Node closing
                    text += 2; // Skip '</'
                    if (flags & parse_flag::validate_closing_tags)
                    {
                        // Skip and validate closing tag name
                        char* closing_name = text;
                        skip<node_name_pred>(text);
                        if (node->name() != string_view(closing_name, text - closing_name))
                            throw parse_error("invalid closing tag name", text);
                    }
                    else
                    {
                        // No validation, just skip name
                        skip<node_name_pred>(text);
                    }
                    // Skip remaining whitespace after node name
                    skip<whitespace_pred>(text);
                    if (*text != '>')
                        throw parse_error("expected >", text);
                    ++text; // Skip '>'
                    return; // Node closed, finished parsing contents
                }
                else
                {
                    // charild node
                    ++text; // Skip '<'
                    if (xml_node* child = parse_node(text, namespace_scope, flags))
                        node->append_node(child);
                }
                break;

            // End of data - error
            case '\0':
                throw parse_error("unexpected end of data", text);

            // Data node
            default:
                next_char = parse_and_append_data(node, text, contents_start, flags);
                goto after_data_node; // Bypass regular processing after data nodes
            }
        }
    }

    void xml_document::parse_node_attributes(char*& text, xml_node* node, parse_flag flags)
    {
        // For all attributes
        while (attribute_ncname_pred::test(*text))
        {
            // Extract attribute name
            char* name = text;
            ++text; // Skip first character of attribute name
            skip<attribute_ncname_pred>(text);
            if (text == name)
                throw parse_error("expected attribute name", name);
            // Create new attribute
            xml_attribute* attribute = pool.allocate_attribute();
            if (*text == ':')
            {
                // Namespace prefix found
                ++text;
                char* local_name = text;
                skip<attribute_ncname_pred>(text);
                if (text == local_name)
                    throw parse_error("expected local part of attribute name", local_name);
                attribute->qname(string_view(name, text - name), local_name - name);
            }
            else
                attribute->qname(string_view(name, text - name));

            node->append_attribute(attribute);

            // Skip whitespace after attribute name
            skip<whitespace_pred>(text);

            // Skip =
            if (*text != '=')
                throw parse_error("expected =", text);
            ++text;

            // Skip whitespace after =
            skip<whitespace_pred>(text);

            // Skip quote and remember if it was ' or "
            char quote = *text;
            if (quote != char('\'') && quote != '"')
                throw parse_error("expected ' or \"", text);
            ++text;

            // Extract attribute value and expand char refs in it
            char *value = text, *end;
            parse_flag AttFlags{ flags & ~parse_flag::normalize_whitespace }; // No whitespace normalization in attributes
            if (quote == char('\''))
                end = skip_and_expand_character_refs<attribute_value_pred<char('\'')>, attribute_value_pure_pred<char('\'')>>(text, AttFlags);
            else
                end = skip_and_expand_character_refs<attribute_value_pred<'"'>, attribute_value_pure_pred<'"'>>(text, AttFlags);

            // Set attribute value
            attribute->value(string_view(value, end - value));

            // Make sure that end quote is present
            if (*text != quote)
                throw parse_error("expected ' or \"", text);
            ++text; // Skip quote

            // Skip whitespace after attribute value
            skip<whitespace_pred>(text);
        }
    }
} // namespace rapidxml

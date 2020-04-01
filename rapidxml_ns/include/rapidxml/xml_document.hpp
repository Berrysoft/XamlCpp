#ifndef RAPID_XML_DOCUMENT_HPP
#define RAPID_XML_DOCUMENT_HPP

#include <rapidxml/memory_pool.hpp>
#include <rapidxml/xml_node.hpp>

namespace rapidxml
{
    //! This class represents root of the DOM hierarchy.
    //! It is also an xml_node and a memory_pool through public inheritance.
    //! Use parse() function to build a DOM tree from a zero-terminated XML text string.
    //! parse() function allocates memory for nodes and attributes by using functions of xml_document,
    //! which are inherited from memory_pool.
    //! To access root node of the document, use the document itself, as if it was an xml_node.
    //! \param char chararacter type to use.
    class xml_document : public xml_node, public memory_pool
    {

    public:
        //! Constructs empty XML document
        xml_document()
            : xml_node(node_document)
        {
        }

        //! Parses zero-terminated XML string according to given flags.
        //! Passed string will be modified by the parser, unless rapidxml_ns::parse_non_destructive flag is used.
        //! The string must persist for the lifetime of the document.
        //! In case of error, rapidxml_ns::parse_error exception will be thrown.
        //! <br><br>
        //! If you want to parse contents of a file, you must first load the file into the memory, and pass pointer to its beginning.
        //! Make sure that data is zero-terminated.
        //! <br><br>
        //! Document can be parsed into multiple times.
        //! Each new call to parse removes previous nodes and attributes (if any), but does not clear memory pool.
        //! \param text XML data to parse; pointer is non-const to denote fact that this data may be modified by the parser.
        template <int Flags>
        void parse(char* text)
        {
            if (Flags & parse_no_namespace)
                parse_ns<Flags, internal::xml_namespace_processor_stub>(text);
            else
                parse_ns<Flags, internal::xml_namespace_processor>(text);
        }

        //! Use parse() instead.
        //! Parses zero-terminated XML string according to given flags and NamespaceProcessor passed.
        //! Should be called only when default xml_namespace_processor is substituted with custom one.
        template <int Flags, class NamespaceProcessor>
        void parse_ns(char* text)
        {
            assert(text);

            // Remove current contents
            this->remove_all_nodes();
            this->remove_all_attributes();

            NamespaceProcessor namespace_processor;
            // Creating topmost namespace scope that actually won't be used
            typename NamespaceProcessor::scope const namespace_scope(namespace_processor);

            // Parse BOM, if any
            parse_bom<Flags>(text);

            // Parse children
            while (1)
            {
                // Skip whitespace before node
                skip<whitespace_pred, Flags>(text);
                if (*text == 0)
                    break;

                // Parse and append new child
                if (*text == char('<'))
                {
                    ++text; // Skip '<'
                    if (xml_node* node = parse_node<Flags, typename NamespaceProcessor::scope>(text, namespace_scope))
                        this->append_node(node);
                }
                else
                    RAPIDXML_PARSE_ERROR("expected <", text);
            }
        }

        //! Clears the document by deleting all nodes and clearing the memory pool.
        //! All nodes owned by document pool are destroyed.
        void clear()
        {
            this->remove_all_nodes();
            this->remove_all_attributes();
            memory_pool::clear();
        }

    private:
        ///////////////////////////////////////////////////////////////////////
        // Internal character utility functions

        // Detect whitespace character
        struct whitespace_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_whitespace[static_cast<unsigned char>(ch)];
            }
        };

        // Detect node name character
        struct node_name_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_node_name[static_cast<unsigned char>(ch)];
            }
        };

        // Detect node name character without ':' (NCName) - namespace prefix or local name
        struct node_ncname_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_node_ncname[static_cast<unsigned char>(ch)];
            }
        };

        // Detect attribute name character
        struct attribute_name_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_attribute_name[static_cast<unsigned char>(ch)];
            }
        };

        // Detect attribute name character without ':' (NCName) - namespace prefix or local name
        struct attribute_ncname_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_attribute_ncname[static_cast<unsigned char>(ch)];
            }
        };

        // Detect text character (PCDATA)
        struct text_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_text[static_cast<unsigned char>(ch)];
            }
        };

        // Detect text character (PCDATA) that does not require processing
        struct text_pure_no_ws_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_text_pure_no_ws[static_cast<unsigned char>(ch)];
            }
        };

        // Detect text character (PCDATA) that does not require processing
        struct text_pure_with_ws_pred
        {
            static unsigned char test(char ch)
            {
                return internal::lookup_tables::lookup_text_pure_with_ws[static_cast<unsigned char>(ch)];
            }
        };

        // Detect attribute value character
        template <char Quote>
        struct attribute_value_pred
        {
            static unsigned char test(char ch)
            {
                if (Quote == char('\''))
                    return internal::lookup_tables::lookup_attribute_data_1[static_cast<unsigned char>(ch)];
                if (Quote == char('\"'))
                    return internal::lookup_tables::lookup_attribute_data_2[static_cast<unsigned char>(ch)];
                return 0; // Should never be executed, to avoid warnings on Comeau
            }
        };

        // Detect attribute value character
        template <char Quote>
        struct attribute_value_pure_pred
        {
            static unsigned char test(char ch)
            {
                if (Quote == char('\''))
                    return internal::lookup_tables::lookup_attribute_data_1_pure[static_cast<unsigned char>(ch)];
                if (Quote == char('\"'))
                    return internal::lookup_tables::lookup_attribute_data_2_pure[static_cast<unsigned char>(ch)];
                return 0; // Should never be executed, to avoid warnings on Comeau
            }
        };

        // Insert coded character, using UTF8 or 8-bit ASCII
        template <int Flags>
        static void insert_coded_character(char*& text, unsigned long code)
        {
            if (Flags & parse_no_utf8)
            {
                // Insert 8-bit ASCII character
                // Todo: possibly verify that code is less than 256 and use replacement char otherwise?
                text[0] = static_cast<unsigned char>(code);
                text += 1;
            }
            else
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
                    RAPIDXML_PARSE_ERROR("invalid numeric character entity", text);
                }
            }
        }

        // Skip characters until predicate evaluates to true
        template <class StopPred, int Flags>
        static void skip(char*& text)
        {
            char* tmp = text;
            while (StopPred::test(*tmp))
                ++tmp;
            text = tmp;
        }

        // Skip characters until predicate evaluates to true while doing the following:
        // - replacing XML character entity references with proper characters (&apos; &amp; &quot; &lt; &gt; &#...;)
        // - condensing whitespace sequences to single space character
        template <class StopPred, class StopPredPure, int Flags>
        static char* skip_and_expand_character_refs(char*& text)
        {
            // If entity translation, whitespace condense and whitespace trimming is disabled, use plain skip
            if (Flags & parse_no_entity_translation &&
                !(Flags & parse_normalize_whitespace) &&
                !(Flags & parse_trim_whitespace))
            {
                skip<StopPred, Flags>(text);
                return text;
            }

            // Use simple skip until first modification is detected
            skip<StopPredPure, Flags>(text);

            // Use translation skip
            char* src = text;
            char* dest = src;
            while (StopPred::test(*src))
            {
                // If entity translation is enabled
                if (!(Flags & parse_no_entity_translation))
                {
                    // Test if replacement is needed
                    if (src[0] == char('&'))
                    {
                        switch (src[1])
                        {

                        // &amp; &apos;
                        case char('a'):
                            if (src[2] == char('m') && src[3] == char('p') && src[4] == char(';'))
                            {
                                *dest = char('&');
                                ++dest;
                                src += 5;
                                continue;
                            }
                            if (src[2] == char('p') && src[3] == char('o') && src[4] == char('s') && src[5] == char(';'))
                            {
                                *dest = char('\'');
                                ++dest;
                                src += 6;
                                continue;
                            }
                            break;

                        // &quot;
                        case char('q'):
                            if (src[2] == char('u') && src[3] == char('o') && src[4] == char('t') && src[5] == char(';'))
                            {
                                *dest = char('"');
                                ++dest;
                                src += 6;
                                continue;
                            }
                            break;

                        // &gt;
                        case char('g'):
                            if (src[2] == char('t') && src[3] == char(';'))
                            {
                                *dest = char('>');
                                ++dest;
                                src += 4;
                                continue;
                            }
                            break;

                        // &lt;
                        case char('l'):
                            if (src[2] == char('t') && src[3] == char(';'))
                            {
                                *dest = char('<');
                                ++dest;
                                src += 4;
                                continue;
                            }
                            break;

                        // &#...; - assumes ASCII
                        case char('#'):
                            if (src[2] == char('x'))
                            {
                                unsigned long code = 0;
                                src += 3; // Skip &#x
                                while (1)
                                {
                                    unsigned char digit = internal::lookup_tables::lookup_digits[static_cast<unsigned char>(*src)];
                                    if (digit == 0xFF)
                                        break;
                                    code = code * 16 + digit;
                                    ++src;
                                }
                                insert_coded_character<Flags>(dest, code); // Put character in output
                            }
                            else
                            {
                                unsigned long code = 0;
                                src += 2; // Skip &#
                                while (1)
                                {
                                    unsigned char digit = internal::lookup_tables::lookup_digits[static_cast<unsigned char>(*src)];
                                    if (digit == 0xFF)
                                        break;
                                    code = code * 10 + digit;
                                    ++src;
                                }
                                insert_coded_character<Flags>(dest, code); // Put character in output
                            }
                            if (*src == char(';'))
                                ++src;
                            else
                                RAPIDXML_PARSE_ERROR("expected ;", src);
                            continue;

                        // Something else
                        default:
                            // Ignore, just copy '&' verbatim
                            break;
                        }
                    }
                }

                // If whitespace condensing is enabled
                if (Flags & parse_normalize_whitespace)
                {
                    // Test if condensing is needed
                    if (whitespace_pred::test(*src))
                    {
                        *dest = char(' ');
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

        ///////////////////////////////////////////////////////////////////////
        // Internal parsing functions

        // Parse BOM, if any
        template <int Flags>
        void parse_bom(char*& text)
        {
            // UTF-8?
            if (static_cast<unsigned char>(text[0]) == 0xEF &&
                static_cast<unsigned char>(text[1]) == 0xBB &&
                static_cast<unsigned char>(text[2]) == 0xBF)
            {
                text += 3; // Skup utf-8 bom
            }
        }

        // Parse XML declaration (<?xml...)
        template <int Flags>
        xml_node* parse_xml_declaration(char*& text)
        {
            // If parsing of declaration is disabled
            if (!(Flags & parse_declaration_node))
            {
                // Skip until end of declaration
                while (text[0] != char('?') || text[1] != char('>'))
                {
                    if (!text[0])
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 2; // Skip '?>'
                return 0;
            }

            // Create declaration
            xml_node* declaration = this->allocate_node(node_declaration);

            // Skip whitespace before attributes or ?>
            skip<whitespace_pred, Flags>(text);

            // Parse declaration attributes
            parse_node_attributes<Flags>(text, declaration);

            // Skip ?>
            if (text[0] != char('?') || text[1] != char('>'))
                RAPIDXML_PARSE_ERROR("expected ?>", text);
            text += 2;

            return declaration;
        }

        // Parse XML comment (<!--...)
        template <int Flags>
        xml_node* parse_comment(char*& text)
        {
            // If parsing of comments is disabled
            if (!(Flags & parse_comment_nodes))
            {
                // Skip until end of comment
                while (text[0] != char('-') || text[1] != char('-') || text[2] != char('>'))
                {
                    if (!text[0])
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 3; // Skip '-->'
                return 0; // Do not produce comment node
            }

            // Remember value start
            char* value = text;

            // Skip until end of comment
            while (text[0] != char('-') || text[1] != char('-') || text[2] != char('>'))
            {
                if (!text[0])
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                ++text;
            }

            // Create comment node
            xml_node* comment = this->allocate_node(node_comment);
            comment->value(value, text - value);

            // Place zero terminator after comment value
            if (!(Flags & parse_no_string_terminators))
                *text = char('\0');

            text += 3; // Skip '-->'
            return comment;
        }

        // Parse DOCTYPE
        template <int Flags>
        xml_node* parse_doctype(char*& text)
        {
            // Remember value start
            char* value = text;

            // Skip to >
            while (*text != char('>'))
            {
                // Determine character type
                switch (*text)
                {

                // If '[' encountered, scan for matching ending ']' using naive algorithm with depth
                // This works for all W3C test files except for 2 most wicked
                case char('['):
                {
                    ++text; // Skip '['
                    int depth = 1;
                    while (depth > 0)
                    {
                        switch (*text)
                        {
                        case char('['):
                            ++depth;
                            break;
                        case char(']'):
                            --depth;
                            break;
                        case 0:
                            RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                        }
                        ++text;
                    }
                    break;
                }

                // Error on end of text
                case char('\0'):
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);

                // Other character, skip it
                default:
                    ++text;
                }
            }

            // If DOCTYPE nodes enabled
            if (Flags & parse_doctype_node)
            {
                // Create a new doctype node
                xml_node* doctype = this->allocate_node(node_doctype);
                doctype->value(value, text - value);

                // Place zero terminator after value
                if (!(Flags & parse_no_string_terminators))
                    *text = char('\0');

                text += 1; // skip '>'
                return doctype;
            }
            else
            {
                text += 1; // skip '>'
                return 0;
            }
        }

        // Parse PI
        template <int Flags>
        xml_node* parse_pi(char*& text)
        {
            // If creation of PI nodes is enabled
            if (Flags & parse_pi_nodes)
            {
                // Create pi node
                xml_node* pi = this->allocate_node(node_pi);

                // Extract PI target name
                char* name = text;
                skip<node_name_pred, Flags>(text);
                if (text == name)
                    RAPIDXML_PARSE_ERROR("expected PI target", text);
                pi->name(name, text - name);

                // Skip whitespace between pi target and pi
                skip<whitespace_pred, Flags>(text);

                // Remember start of pi
                char* value = text;

                // Skip to '?>'
                while (text[0] != char('?') || text[1] != char('>'))
                {
                    if (*text == char('\0'))
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }

                // Set pi value (verbatim, no entity expansion or whitespace normalization)
                pi->value(value, text - value);

                // Place zero terminator after name and value
                if (!(Flags & parse_no_string_terminators))
                {
                    pi->name()[pi->name_size()] = char('\0');
                    pi->value()[pi->value_size()] = char('\0');
                }

                text += 2; // Skip '?>'
                return pi;
            }
            else
            {
                // Skip to '?>'
                while (text[0] != char('?') || text[1] != char('>'))
                {
                    if (*text == char('\0'))
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 2; // Skip '?>'
                return 0;
            }
        }

        // Parse and append data
        // Return character that ends data.
        // This is necessary because this character might have been overwritten by a terminating 0
        template <int Flags>
        char parse_and_append_data(xml_node* node, char*& text, char* contents_start)
        {
            // Backup to contents start if whitespace trimming is disabled
            if (!(Flags & parse_trim_whitespace))
                text = contents_start;

            // Skip until end of data
            char *value = text, *end;
            if (Flags & parse_normalize_whitespace)
                end = skip_and_expand_character_refs<text_pred, text_pure_with_ws_pred, Flags>(text);
            else
                end = skip_and_expand_character_refs<text_pred, text_pure_no_ws_pred, Flags>(text);

            // Trim trailing whitespace if flag is set; leading was already trimmed by whitespace skip after >
            if (Flags & parse_trim_whitespace)
            {
                if (Flags & parse_normalize_whitespace)
                {
                    // Whitespace is already condensed to single space characters by skipping function, so just trim 1 char off the end
                    if (*(end - 1) == char(' '))
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
            if (!(Flags & parse_no_data_nodes))
            {
                xml_node* data = this->allocate_node(node_data);
                data->value(value, end - value);
                node->append_node(data);
            }

            // Add data to parent node if no data exists yet
            if (!(Flags & parse_no_element_values))
                if (*node->value() == char('\0'))
                    node->value(value, end - value);

            // Place zero terminator after value
            if (!(Flags & parse_no_string_terminators))
            {
                char ch = *text;
                *end = char('\0');
                return ch; // Return character that ends data; this is required because zero terminator overwritten it
            }

            // Return character that ends data
            return *text;
        }

        // Parse CDATA
        template <int Flags>
        xml_node* parse_cdata(char*& text)
        {
            // If CDATA is disabled
            if (Flags & parse_no_data_nodes)
            {
                // Skip until end of cdata
                while (text[0] != char(']') || text[1] != char(']') || text[2] != char('>'))
                {
                    if (!text[0])
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 3; // Skip ]]>
                return 0; // Do not produce CDATA node
            }

            // Skip until end of cdata
            char* value = text;
            while (text[0] != char(']') || text[1] != char(']') || text[2] != char('>'))
            {
                if (!text[0])
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                ++text;
            }

            // Create new cdata node
            xml_node* cdata = this->allocate_node(node_cdata);
            cdata->value(value, text - value);

            // Place zero terminator after value
            if (!(Flags & parse_no_string_terminators))
                *text = char('\0');

            text += 3; // Skip ]]>
            return cdata;
        }

        // Parse element node
        template <int Flags, class NamespaceScope>
        xml_node* parse_element(char*& text, NamespaceScope namespace_scope)
        {
            // Create element node
            xml_node* element = this->allocate_node(node_element);

            // Extract element name
            char* name = text;
            if (Flags & parse_no_namespace)
                skip<node_name_pred, Flags>(text);
            else
                skip<node_ncname_pred, Flags>(text);
            if (text == name)
                RAPIDXML_PARSE_ERROR("expected element name", text);
            if ((Flags & parse_no_namespace) == 0 && *text == char(':'))
            {
                // Namespace prefix found
                ++text;
                char* local_name = text;
                skip<node_ncname_pred, Flags>(text);
                if (*text == char(':'))
                    RAPIDXML_PARSE_ERROR("second colon in element name", text);
                if (text == local_name)
                    RAPIDXML_PARSE_ERROR("expected local part of element name", text);
                element->qname(name, text - name, local_name);
            }
            else
                element->qname(name, text - name);

            // Skip whitespace between element name and attributes or >
            skip<whitespace_pred, Flags>(text);

            // Parse attributes, if any
            parse_node_attributes<Flags>(text, element);

            // Setting attributes and element own namespace_uri, adding declared
            // namespace prefixes and probably setting default namespace
            namespace_scope.process_element(element);

            // Determine ending type
            if (*text == char('>'))
            {
                ++text;
                parse_node_contents<Flags, NamespaceScope>(text, element, namespace_scope);
            }
            else if (*text == char('/'))
            {
                ++text;
                if (*text != char('>'))
                    RAPIDXML_PARSE_ERROR("expected >", text);
                ++text;
            }
            else
                RAPIDXML_PARSE_ERROR("expected >", text);

            // Place zero terminator after name
            if (!(Flags & parse_no_string_terminators))
                element->name()[element->name_size()] = char('\0');

            // Return parsed element
            return element;
        }

        // Determine node type, and parse it
        template <int Flags, class NamespaceScope>
        xml_node* parse_node(char*& text, NamespaceScope const& namespace_scope)
        {
            // Parse proper node type
            switch (text[0])
            {

            // <...
            default:
                // Parse and append element node
                return parse_element<Flags, NamespaceScope>(text, namespace_scope);

            // <?...
            case char('?'):
                ++text; // Skip ?
                if ((text[0] == char('x') || text[0] == char('X')) &&
                    (text[1] == char('m') || text[1] == char('M')) &&
                    (text[2] == char('l') || text[2] == char('L')) &&
                    whitespace_pred::test(text[3]))
                {
                    // '<?xml ' - xml declaration
                    text += 4; // Skip 'xml '
                    return parse_xml_declaration<Flags>(text);
                }
                else
                {
                    // Parse PI
                    return parse_pi<Flags>(text);
                }

            // <!...
            case char('!'):

                // Parse proper subset of <! node
                switch (text[1])
                {

                // <!-
                case char('-'):
                    if (text[2] == char('-'))
                    {
                        // '<!--' - xml comment
                        text += 3; // Skip '!--'
                        return parse_comment<Flags>(text);
                    }
                    break;

                // <![
                case char('['):
                    if (text[2] == char('C') && text[3] == char('D') && text[4] == char('A') &&
                        text[5] == char('T') && text[6] == char('A') && text[7] == char('['))
                    {
                        // '<![CDATA[' - cdata
                        text += 8; // Skip '![CDATA['
                        return parse_cdata<Flags>(text);
                    }
                    break;

                // <!D
                case char('D'):
                    if (text[2] == char('O') && text[3] == char('C') && text[4] == char('T') &&
                        text[5] == char('Y') && text[6] == char('P') && text[7] == char('E') &&
                        whitespace_pred::test(text[8]))
                    {
                        // '<!DOCTYPE ' - doctype
                        text += 9; // skip '!DOCTYPE '
                        return parse_doctype<Flags>(text);
                    }

                } // switch

                // Attempt to skip other, unrecognized node types starting with <!
                ++text; // Skip !
                while (*text != char('>'))
                {
                    if (*text == 0)
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                ++text; // Skip '>'
                return 0; // No node recognized
            }
        }

        // Parse contents of the node - children, data etc.
        template <int Flags, class NamespaceScope>
        void parse_node_contents(char*& text, xml_node* node, NamespaceScope const& namespace_scope)
        {
            // For all children and text
            while (1)
            {
                // Skip whitespace between > and node contents
                char* contents_start = text; // Store start of node contents before whitespace is skipped
                skip<whitespace_pred, Flags>(text);
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
                case char('<'):
                    if (text[1] == char('/'))
                    {
                        // Node closing
                        text += 2; // Skip '</'
                        if (Flags & parse_validate_closing_tags)
                        {
                            // Skip and validate closing tag name
                            char* closing_name = text;
                            skip<node_name_pred, Flags>(text);
                            if (!internal::compare(node->name(), node->name_size(), closing_name, text - closing_name, true))
                                RAPIDXML_PARSE_ERROR("invalid closing tag name", text);
                        }
                        else
                        {
                            // No validation, just skip name
                            skip<node_name_pred, Flags>(text);
                        }
                        // Skip remaining whitespace after node name
                        skip<whitespace_pred, Flags>(text);
                        if (*text != char('>'))
                            RAPIDXML_PARSE_ERROR("expected >", text);
                        ++text; // Skip '>'
                        return; // Node closed, finished parsing contents
                    }
                    else
                    {
                        // charild node
                        ++text; // Skip '<'
                        if (xml_node* child = parse_node<Flags, NamespaceScope>(text, namespace_scope))
                            node->append_node(child);
                    }
                    break;

                // End of data - error
                case char('\0'):
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);

                // Data node
                default:
                    next_char = parse_and_append_data<Flags>(node, text, contents_start);
                    goto after_data_node; // Bypass regular processing after data nodes
                }
            }
        }

        // Parse XML attributes of the node
        template <int Flags>
        void parse_node_attributes(char*& text, xml_node* node)
        {
            // For all attributes
            while ((Flags & parse_no_namespace) ? attribute_name_pred::test(*text) : attribute_ncname_pred::test(*text))
            {
                // Extract attribute name
                char* name = text;
                ++text; // Skip first character of attribute name
                if (Flags & parse_no_namespace)
                    skip<attribute_name_pred, Flags>(text);
                else
                    skip<attribute_ncname_pred, Flags>(text);
                if (text == name)
                    RAPIDXML_PARSE_ERROR("expected attribute name", name);
                // Create new attribute
                xml_attribute* attribute = this->allocate_attribute();
                if ((Flags & parse_no_namespace) == 0 && *text == char(':'))
                {
                    // Namespace prefix found
                    ++text;
                    char* local_name = text;
                    skip<attribute_ncname_pred, Flags>(text);
                    if (text == local_name)
                        RAPIDXML_PARSE_ERROR("expected local part of attribute name", local_name);
                    attribute->qname(name, text - name, local_name);
                }
                else
                    attribute->qname(name, text - name);

                node->append_attribute(attribute);

                // Skip whitespace after attribute name
                skip<whitespace_pred, Flags>(text);

                // Skip =
                if (*text != char('='))
                    RAPIDXML_PARSE_ERROR("expected =", text);
                ++text;

                // Add terminating zero after name
                if (!(Flags & parse_no_string_terminators))
                    attribute->name()[attribute->name_size()] = 0;

                // Skip whitespace after =
                skip<whitespace_pred, Flags>(text);

                // Skip quote and remember if it was ' or "
                char quote = *text;
                if (quote != char('\'') && quote != char('"'))
                    RAPIDXML_PARSE_ERROR("expected ' or \"", text);
                ++text;

                // Extract attribute value and expand char refs in it
                char *value = text, *end;
                const int AttFlags = Flags & ~parse_normalize_whitespace; // No whitespace normalization in attributes
                if (quote == char('\''))
                    end = skip_and_expand_character_refs<attribute_value_pred<char('\'')>, attribute_value_pure_pred<char('\'')>, AttFlags>(text);
                else
                    end = skip_and_expand_character_refs<attribute_value_pred<char('"')>, attribute_value_pure_pred<char('"')>, AttFlags>(text);

                // Set attribute value
                attribute->value(value, end - value);

                // Make sure that end quote is present
                if (*text != quote)
                    RAPIDXML_PARSE_ERROR("expected ' or \"", text);
                ++text; // Skip quote

                // Add terminating zero after value
                if (!(Flags & parse_no_string_terminators))
                    attribute->value()[attribute->value_size()] = 0;

                // Skip whitespace after attribute value
                skip<whitespace_pred, Flags>(text);
            }
        }
    };
} // namespace rapidxml

#endif // !RAPID_XML_DOCUMENT_HPP

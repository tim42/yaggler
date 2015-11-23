//
// file : shader_preprocessor.hpp
// in : file:///home/tim/projects/yaggler/yaggler/shader/shader_preprocessor.hpp
//
// created by : Timothée Feuillet on linux-vnd3.site
// date: 19/09/2015 11:58:54
//
//
// Copyright (C) 2014 Timothée Feuillet
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#ifndef __N_7059748521900707419_246203373__SHADER_PREPROCESSOR_HPP__
# define __N_7059748521900707419_246203373__SHADER_PREPROCESSOR_HPP__

#include <string>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace neam
{
  namespace yaggler
  {
    namespace shader
    {
      /// \brief A very simple preprocessor lexer&parser
      /// It don't do any other action than listing preprocessor declaration per type (like defines, version, ...)
      /// It also permit to replace and insert some content in the file.
      ///
      /// This is a handwritten lexer & ~parser, working on a line by line basis
      class shader_preprocessor
      {
        public:
          enum class e_token_type
          {
            none, // not really used
            code, // we don't touch that
            single_line_comment, // could be removed
            multiline_comment, // could be removed
            preprocessor_token, // The full preprocessor line
            preprocessor_token_args, // (a, v, x)
            preprocessor_token_string, // "bla bla bla" or <bla bla bla>
          };

          struct token_entry
          {
            size_t uid;
            e_token_type type; ///< \brief the token type
            size_t line;       ///< \brief the line number
            std::string str;   ///< \brief the content of the token

            std::list<token_entry> subtokens; ///< \brief for preprocessor tokens only

            bool operator == (const token_entry &o)
            {
              return o.uid == this->uid &&
                     o.type == this->type &&
                     o.line == this->line &&
                     o.str == this->str;
            }
          };
          using entry_map = std::map<std::string, std::list<token_entry>>;

        public:
          /// \brief constructor
          shader_preprocessor() {}

          /// \brief construct the object by feeding it a shader content
          explicit shader_preprocessor(const std::string &content)
          {
            feed(content);
          }

          /// \brief destructor
          ~shader_preprocessor() {}

          /// \brief Expand the shader code with another content
          /// \param content The content to add to the shader
          /// \param filename A filename for the shader
          void feed(const std::string &content)
          {
            std::list<token_entry> fed_tokens = tokenize(content);
            parse(fed_tokens, entries);
            if (!tokens.size())
              tokens.swap(fed_tokens);
            else
              tokens.insert(tokens.end(), fed_tokens.begin(), fed_tokens.end());
          }

          /// \brief Replace a token with a given content
          std::list<token_entry> replace_token(const token_entry &token, const std::string &content)
          {
            // okay, slower than having both done at the same time,
            // but it avoid some code duplication.
            std::list<token_entry> ret = insert_after(token, content);
            remove_token(token);
            return ret;
          }

          /// \brief insert something after a given token
          std::list<token_entry> insert_after(const token_entry &token, const std::string &content)
          {
            auto it = std::find(tokens.begin(), tokens.end(), token);
            if (it == tokens.end())
              return std::list<token_entry>();

            std::list<token_entry> tmp_tokens = tokenize(content);
            tokens.insert(++it, tmp_tokens.begin(), tmp_tokens.end());
            parse(tmp_tokens, entries);
            return tmp_tokens;
          }

          /// \brief insert something before a given token
          std::list<token_entry> insert_before(const token_entry &token, const std::string &content)
          {
            auto it = std::find(tokens.begin(), tokens.end(), token);
            if (it == tokens.end())
              return std::list<token_entry>();

            std::list<token_entry> tmp_tokens = tokenize(content);
            tokens.insert(it, tmp_tokens.begin(), tmp_tokens.end());
            parse(tmp_tokens, entries);
            return tmp_tokens;
          }

          /// \brief Get the tokens corresponding to a preprocessor directive
          /// \param preprocessor_directive is the name of the directive (like "define" or "include")
          /// \return the list of tokens
          const std::list<token_entry> &get_tokens(const std::string &preprocessor_directive) const
          {
            auto it = entries.find(preprocessor_directive);
            if (it == entries.end())
              return empty_list;
            return it->second;
          }

          /// \brief Get the tokens corresponding to a preprocessor directive and a specific key
          /// \param preprocessor_directive is the name of the directive (like "define" or "include")
          /// \param key the key (for "define" it will be the name of the macro being defined, without any argument)
          /// \return the list of tokens
          const std::list<token_entry> get_tokens(const std::string &preprocessor_directive, const std::string &key) const
          {
            std::list<token_entry> ret;
            auto it = entries.find(preprocessor_directive);
            if (it == entries.end())
              return ret;
            for (const token_entry &entry : it->second)
            {
              if (entry.subtokens.size() >= 2 && (++entry.subtokens.begin())->str == key)
                ret.push_back(entry);
            }
            return ret;
          }

          /// \brief Get the first token matching a preprocessor directive
          /// \param preprocessor_directive is the name of the directive (like "define" or "include")
          /// \return the first token matching it or a special type == none token
          token_entry get_first_token(const std::string &preprocessor_directive) const
          {
            auto it = entries.find(preprocessor_directive);
            if (it == entries.end() || !it->second.size())
              return token_entry {0, e_token_type::none, 0, "", {}};
            return it->second.front();
          }

          /// \brief Return a single token matching both preprocessor_directive and key
          /// \param preprocessor_directive is the name of the directive (like "define" or "include")
          /// \param key the key (for "define" it will be the name of the macro being defined, without any argument)
          /// \return the token if found or a special type == none token
          token_entry get_token(const std::string &preprocessor_directive, const std::string &key) const
          {
            if (!key.size())
              return get_first_token(preprocessor_directive);
            auto it = entries.find(preprocessor_directive);
            if (it != entries.end())
            {
              auto token_it = std::find_if(it->second.begin(), it->second.end(), [&key](const token_entry &entry)
              {
                return entry.subtokens.size() >= 2 && (++entry.subtokens.begin())->str == key;
              });

              if (token_it != it->second.end())
                return *token_it;
            }

            return token_entry {0, e_token_type::none, 0, "", {}};
          }

          /// \brief remove a token
          /// \param token the token to remove
          void remove_token(const token_entry &token)
          {
            // remove the token from the entries map
            if (token.subtokens.size())
            {
              auto it = entries.find(token.subtokens.front().str);
              if (token.type == e_token_type::preprocessor_token && it != entries.end())
                it->second.remove(token);
            }

            // finally remove the token from the token list
            tokens.remove(token);
          }
          /// \brief remove a token range ]from,to[
          /// \param from the token before those to remove
          /// \param to the token after those to remove
          void remove_token_range(const token_entry &from, const token_entry &to)
          {
            // get range tokens
            auto range_it_from = std::find(tokens.begin(), tokens.end(), from);
            auto range_it_to = std::find(tokens.begin(), tokens.end(), to);

            if (range_it_from == tokens.end())
              return;
            ++range_it_from;
            if (range_it_from == range_it_to)
              return;

            for (auto range_it = range_it_from; range_it != range_it_to; ++range_it)
            {
              if (!range_it->subtokens.size())
                continue;
              // remove the tokens from the entries map
              auto it = entries.find(range_it->subtokens.front().str);
              if (range_it->type == e_token_type::preprocessor_token && it != entries.end())
                it->second.remove(*range_it);
            }

            // finally remove the token from the token list
            tokens.erase(range_it_from, range_it_to);
          }

          /// \brief Empty the preprocessor
          void clear()
          {
            entries.clear();
            tokens.clear();
          }

          /// \brief Return a std::string with the shader contents
          /// \param with_comments Whether or not the function should return a string with comments included
          std::string get_string(bool with_comments = true) const
          {
            std::string ret;
            ret.reserve(tokens.size() * 80);
            bool prev_need_spc = false;

            for (const auto &token : tokens)
            {
              if (!with_comments && (token.type == e_token_type::single_line_comment || token.type == e_token_type::multiline_comment))
              {
                prev_need_spc = (token.type == e_token_type::single_line_comment);
                continue;
              }
              if (prev_need_spc)
                ret.push_back(' ');
              prev_need_spc = false;
              if (token.type == e_token_type::single_line_comment)
                ret += "// ";
              if (token.type == e_token_type::multiline_comment)
                ret += "/* ";
              ret += token.str;
              if (token.type == e_token_type::multiline_comment)
                ret += " */";
              else
                ret.push_back('\n');
            }

            return ret;
          }

        private:
          /// \brief Hand written lexer that extract comments, preprocessor
          /// \param content The input string
          /// \return The token list
          std::list<token_entry> tokenize(const std::string &content)
          {
            char next;
            size_t line = 0; // The line counter

            // A special flag to indicate whether or not this multiline comment is handled by the second preprocessor
            bool mulitline_in_preprocessor = false;

            std::list<token_entry> tks;
            size_t initial_line = 0;
            size_t initial_index = 0;
            e_token_type current_type = e_token_type::none;

            for (size_t i = 0, l = content.size(); i < l; ++i)
            {
              next = i + 1 < l ? content[i + 1] : '\0';
              switch (content[i])
              {
                case ' ':
                case '\t':
                  if (current_type == e_token_type::none) // skip whitespaces
                    initial_index = i + 1;
                  break;
                case '/': // test for comments
                  if ((next == '/' || next == '*') && current_type != e_token_type::multiline_comment
                      && current_type != e_token_type::single_line_comment)
                  {
                    if (next == '*' && current_type == e_token_type::preprocessor_token) // We handle them in the second lexer
                    {
                      ++i; // we skip next
                      mulitline_in_preprocessor = true;
                      break;
                    }
                    if (current_type != e_token_type::none) // Create a new token
                      tks.emplace_back(token_entry {++uid_counter, current_type, initial_line, content.substr(initial_index, i - initial_index), {}});

                    if (next == '*')
                      current_type = e_token_type::multiline_comment;
                    else
                      current_type = e_token_type::single_line_comment;

                    ++i; // we skip next
                    initial_line = line;
                    initial_index = i + 1;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::code;
                  break;
                case '*': // test for the end of multiline comments
                  if (next == '/' && current_type == e_token_type::multiline_comment) // end comment
                  {
                    tks.emplace_back(token_entry {++uid_counter, current_type, initial_line, content.substr(initial_index, i - 1 - initial_index), {}});
                    ++i; // we skip next
                    initial_index = i + 1;
                    initial_line = line;
                    current_type = e_token_type::none;
                  }
                  else if (next == '/' && current_type == e_token_type::preprocessor_token && mulitline_in_preprocessor)
                  {
                    ++i;
                    mulitline_in_preprocessor = false;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::code;
                  break;
                case '#': // test for preprocessor
                  if (current_type == e_token_type::none)
                  {
                    current_type = e_token_type::preprocessor_token;
                    initial_index = i;
                  }
                  break;
                case '\\': // test for \\\n
                  if (next == '\n') // skip newline
                  {
                    ++i; // we skip next
                    ++line; // as we skip next, we incr the line counter
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::code;
                  break;
                case '\n':
                  ++line;
                  if (mulitline_in_preprocessor) // We are still in a comment but handled by the second lexer
                    break;
                  if (current_type != e_token_type::none && current_type != e_token_type::multiline_comment)
                  {
                    // We have accumulated some data to create a new token
                    tks.emplace_back(token_entry {++uid_counter, current_type, initial_line, content.substr(initial_index, i - initial_index), {}});
                    current_type = e_token_type::none;
                    initial_index = i + 1;
                    initial_line = line;
                  }
                  else if (current_type == e_token_type::none)
                  {
                    initial_index = i + 1;
                    initial_line = line;
                  }
                  break;
                default:
                  if (current_type == e_token_type::none)
                    current_type = e_token_type::code;
              }
            }

            // We may have accumulated some data to create a new token
            if (current_type != e_token_type::none)
              tks.emplace_back(token_entry {++uid_counter, current_type, initial_line, content.substr(initial_index), {}});
            return tks;
          }

          /// \brief The second lexer, called only on preprocessor directives
          /// \note preprocessor directives can still include multiline comments and they are treated as being spaces
          std::list<token_entry> tokenize_preprocessor(const token_entry &token)
          {
            size_t initial_index = 0;
            std::list<token_entry> pp_tks;
            e_token_type current_type = e_token_type::none;

            for (size_t i = 0, l = token.str.size(); i < l; ++i)
            {
              const char next = i + 1 < l ? token.str[i + 1] : '\0';

              switch (token.str[i])
              {
                case '\\':
                  if (next == '\n')
                    ++i; // We skip next
                  break;
                case ' ':
                case '\t':
                case '\n':
                  if (current_type != e_token_type::multiline_comment &&
                      current_type != e_token_type::none &&
                      current_type != e_token_type::preprocessor_token_args &&
                      current_type != e_token_type::preprocessor_token_string)
                  {
                    pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index, i - initial_index), {}});
                    initial_index = i + 1;
                    current_type = e_token_type::none;
                  }
                  else if (current_type == e_token_type::none)
                    initial_index = i + 1;
                  break;
                case '"': // handle strings
                  if (current_type == e_token_type::none)
                  {
                    initial_index = i + 1;
                    current_type = e_token_type::preprocessor_token_string;
                  }
                  else if (current_type == e_token_type::preprocessor_token_string)
                  {
                    pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index, i - initial_index), {}});
                    initial_index = i + 1;
                    current_type = e_token_type::none;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                case '/': // test for multiline comments (treated as whitespace)
                  if (next == '*' && current_type != e_token_type::preprocessor_token_string && current_type != e_token_type::multiline_comment)
                  {
                    if (current_type != e_token_type::none)
                      pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index, i - initial_index), {}});
                    current_type = e_token_type::multiline_comment;
                    ++i;
                    initial_index = i + 1;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                case '*':
                  if (next == '/' && current_type == e_token_type::multiline_comment)
                  {
                    ++i;
                    initial_index = i + 1;
                    current_type = e_token_type::none;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                case '(': // handle brackets
                  if (current_type == e_token_type::preprocessor_token)
                  {
                    pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index, i - initial_index), {}});
                    initial_index = i + 1;
                    current_type = e_token_type::preprocessor_token_args;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                case ',':
                  if (current_type == e_token_type::preprocessor_token_args)
                  {
                    pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index, i - initial_index), {}});
                    initial_index = i + 1;
                    current_type = e_token_type::preprocessor_token_args;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                case ')':
                  if (current_type == e_token_type::preprocessor_token_args)
                  {
                    pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index, i - initial_index), {}});
                    initial_index = i + 1;
                    current_type = e_token_type::none;
                  }
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                case '#':
                  if (pp_tks.size() == 0 && current_type == e_token_type::none)
                    initial_index = i + 1;
                  else if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
                  break;
                default:
                  if (current_type == e_token_type::none)
                    current_type = e_token_type::preprocessor_token;
              }
            }

            // We may have accumulated some data to create a new token
            if (current_type != e_token_type::none && current_type != e_token_type::multiline_comment)
              pp_tks.emplace_back(token_entry {++uid_counter, current_type, token.line, token.str.substr(initial_index), {}});

            return pp_tks;
          }

          /// \brief Briefly "parse" tokens (send preprocessor token to a subsequent lexer and store them)
          /// \param tks The token list as returned by tokenize()
          entry_map &parse(std::list<token_entry> &tks, entry_map &pp_entries)
          {
            for (auto &token : tks)
            {
              if (token.type == e_token_type::preprocessor_token)
              {
                std::list<token_entry> pp_tks = tokenize_preprocessor(token);
                token.subtokens.swap(pp_tks);
                if (token.subtokens.size())
                {
                  if (!pp_entries.count(token.subtokens.front().str))
                    pp_entries[token.subtokens.front().str] = std::list<token_entry>({token});
                  else
                    pp_entries[token.subtokens.front().str].emplace_back(token);
                }
              }
            }

            return pp_entries;
          }

        private:


        private:
          std::list<token_entry> tokens;
          entry_map entries;
          size_t uid_counter = 0;
          const std::list<token_entry> empty_list;
      };
    } // namespace shader
  } // namespace yaggler
} // namespace neam

#endif /*__N_7059748521900707419_246203373__SHADER_PREPROCESSOR_HPP__*/

// kate: indent-mode cstyle; indent-width 2; replace-tabs on; 



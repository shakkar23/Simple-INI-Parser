/*
 * SimpleIniParser
 * Copyright (c) 2019 Steven Mattera
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "IniSection.hpp"
#include <algorithm>

using namespace std;

namespace simpleIniParser {
    IniSection::IniSection(IniSectionType t, string v) {
        type = t;
        value = v;
    }

    IniSection::~IniSection() {
        for (IniOption * option : options) {
            if (option != nullptr) {
                delete option;
                option = nullptr;
            }
        }
        options.clear();
    }

    IniOption * IniSection::findFirstOption(string key) {
        auto it = find_if(options.begin(), options.end(), [&key](const IniOption * obj) { return obj->key == key; });
        if (it == options.end())
            return nullptr;

        return (*it);
    }

    string IniSection::build() {
        if (type == HEKATE_CAPTION)
            return "{" + value + "}\n";
            
        if (type == SEMICOLON_COMMENT)
            return ";" + value + "\n";
            
        if (type == HASHTAG_COMMENT)
            return "#" + value + "\n";

        if (type == BLANK_LINE)
            return "\n";

        string result = "[" + value + "]\n";

        for (IniOption * option : options) {
            result += option->build() + "\n";
        }

        return result;
    }

    IniSection * IniSection::parse(string line) {
        if (line.at(0) == '{' && line.at(line.size() - 1) == '}') {
            return new IniSection(HEKATE_CAPTION, line.substr(1, line.size() - 2));
        } else if (line.at(0) == ';') {
            return new IniSection(SEMICOLON_COMMENT, line.substr(1, line.size() - 1));
        } else if (line.at(0) == '#') {
            return new IniSection(HASHTAG_COMMENT, line.substr(1, line.size() - 1));
        } else if (line.at(0) == '[' && line.at(line.size() - 1) == ']') {
            return new IniSection(SECTION, line.substr(1, line.size() - 2));
        } else if (line.size() == 0) {
            return new IniSection(BLANK_LINE, "");
        } else {
            return nullptr;
        }
    }
}
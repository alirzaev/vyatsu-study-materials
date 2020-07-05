#pragma once
#include "includes.hpp"
#include "types.hpp"
#include "operations.hpp"

namespace risc {
    const size_type REG_OFFSET = 256;
    const size_type INT_OFFSET = REG_OFFSET + 8;

    std::vector<uint8_t> memory(520, 0);
    std::map<std::string, size_type> labels;
    std::map<uint8_t, size_type> constants;
    std::vector<command> program;
    std::vector<token_str> input;
    std::vector<std::string> source;
    std::map<size_type, size_type> debug;
    std::map<std::string, operation> operations = {
        { "in",   &in },
        { "out",  &out },
        { "not",  &bitwise_not },
        { "add",  &add },
        { "sub",  &sub },
        { "jz",   &jz },
        { "jo",   &jo },
        { "xor",  &bitwise_xor },
        { "ror",  &ror },
        { "rol",  &rol },
        { "or",   &bitwise_or },
        { "and",  &bitwise_and },
        { "nand", &bitwise_nand },
        { "nor",  &bitwise_nor }
    };

    bool isdelim(char c) {
        return !isalnum(c) &&
            c != '[' &&
            c != ']' &&
            c != '_' &&
            c != ':';
    }

    size_type get_int_addr(uint8_t n) {
        auto pos = constants.size() + INT_OFFSET;
        if (constants.find(n) == constants.end()) {
            constants[n] = pos;
            memory[pos] = n;
            return pos;
        }
        else {
            return constants[n];
        }
    }

    uint8_t parse_int(const std::string &s) {
        uint8_t n = 0;
        std::pair<std::regex, int> patterns[] = {
            { std::regex(R"(0b([01]+))"), 2 }, // 0b101
            { std::regex(R"((0x[[:xdigit:]]+))"), 16 }, //0xFF
            { std::regex(R"(([0-9]+))"), 10 } // 255
        };
        std::smatch match;
        for (const auto &p : patterns) {
            if (std::regex_search(s, match, p.first)) {
                n = (uint8_t) std::stoul((std::string) match.str(1), nullptr, p.second);
                break;
            }
        }
        return n;
    }

    token_str tokenise(std::string s) {
        // replace commas to spaces
        std::replace(s.begin(), s.end(), ',', ' ');
        // remove comments
        auto p = s.find(';');
        std::string comment("");
        if (p != std::string::npos) {
            comment = s.substr(p, s.length());
            s = s.substr(0, p);
        }
        // tokenise
        token_str tokens;
        std::istringstream tokeniser(s);
        std::string token;
        while (tokeniser >> token) {
            tokens.push_back(token);
        }
        /*std::string token;
        auto c = s.cbegin();
        do {
            if (!isdelim(*c)) {
                token += *c;
                ++c;
            }
            else {
                while (c != s.cend() && isdelim(*c)) {
                    ++c;
                }
                if (!token.empty()) {
                    tokens.push_back(std::move(token));
                    token.clear();
                }
            }
        } while (c != s.cend());
        if (!token.empty()) {
            tokens.push_back(std::move(token));
            token.clear();
        }*/
        tokens.push_back(comment);
        return tokens;
    }

    void make_label_table(const std::vector<token_str> &lines) {
        for (size_type i = 0, pos = 0; i < lines.size(); ++i) {
            // empty string or comment
            const token_str& cmd = lines[i];
            if (cmd.empty() ||
                cmd[0].empty() ||
                cmd[0].front() == ';') {
                continue;
            }
            // label
            if (cmd[0].back() == ':') {
                std::string name(cmd[0].begin(), cmd[0].end() - 1);
                labels[name] = pos;
            }
            // mnemonic
            else {
                input.push_back(cmd);
                debug[pos] = i;
                ++pos;
            }
        }
    }

    void parse_input(void) {
        std::regex label_re(R"([A-Za-z_][A-Za-z0-9_]*)"),
                   register_re(R"(r([0-7]))"),
                   cell_relative_re(R"(\[r([0-7])\])"),
                   cell_abs_re(R"(\[()"
                                  R"(0b[01]+|)"         // [0b1010]
                                  R"(0x[[:xdigit:]]+|)" // [0xFF]
                                  R"([0-9]+)"           // [255]
                                     R"()\])"),
                   literal_re(R"(0b[01]+|)"             // 0b1010
                                 R"(0x[[:xdigit:]]+|)"  // 0xFF
                                 R"([0-9]+)");          // 255
        std::smatch match;
        std::array<cell, 3> addresses;
        for (size_type pos = 0; pos < input.size(); ++pos) {
            const auto &c = input[pos];
            addresses.fill({ 0, val_types::NONE });
            if (operations.find(c[0]) == operations.end()) {
                throw parse_exception(source[debug[pos]]);
            }
            for (size_type i = 1; i < c.size() - 1 && i < 4; ++i) {
                // r0, r1, .. r7
                if (std::regex_match(c[i], match, register_re)) {
                    char r = match.str(1)[0];
                    addresses[i - 1] = { r - '0' + REG_OFFSET, val_types::REG };
                }
                // [r0], [r1], .. [r7]
                else if (std::regex_match(c[i], match, cell_relative_re)) {
                    char r = match.str(1)[0];
                    addresses[i - 1] = { r - '0' + REG_OFFSET, val_types::REL };
                }
                // [0], [1], .. [255]
                else if (std::regex_match(c[i], match, cell_abs_re)) {
                    uint8_t num = parse_int(match.str(1));
                    addresses[i - 1] = { num, val_types::ABS };
                }
                // 0, 1, 2, .. 255
                else if (std::regex_match(c[i], match, literal_re)) {
                    uint8_t num = parse_int(match.str(0));
                    addresses[i - 1] = { get_int_addr(num), val_types::VAL };
                }
                // label1, label2, .. labelN
                else if (std::regex_match(c[i], match, label_re)
                      && labels.find(c[i]) != labels.end()) {
                    addresses[i - 1] = { labels.at(c[i]), val_types::LBL };
                }
                else {
                    throw parse_exception(source[debug[pos]]);
                }
            }
            program.emplace_back(c[0], addresses);
        }
    }

    void load_from_file(std::ifstream& file) {
         std::vector<token_str> tokenised;
         std::string line;
         while (getline(file, line)) {
             tokenised.emplace_back(tokenise(line));
             source.push_back(line);
         }
         make_label_table(tokenised);
         parse_input();
    }

    uint32_t calc_time(const command &cmd) {
        uint32_t t = 1;
        for (const auto& op : cmd.addrs) {
            switch (op.type) {
            case val_types::ABS:
                t += 8;
                break;
            case val_types::REL:
                t += 9;
                break;
            case val_types::REG:
                t += 1;
                break;
            default:
                ;
            }
        }
        return t;
    }

    void print_time(size_type p, uint32_t uptime) {
        uint32_t last = 0;
        if (p <= program.size() && p > 0) {
            last = calc_time(program[p - 1]);
            std::cout << "last command: " << last << std::endl;
        }
        std::cout << "all: " << uptime << std::endl;
    }

    void print_regs() {
        for (size_type i = REG_OFFSET; i != REG_OFFSET + 8; ++i) {
            printf("REG %u: %" PRIu8 "\n", (unsigned) (i - REG_OFFSET), memory[i]);
        }
        printf("\n");
    }

    void print_mem_range(size_type begin, size_type end) {
        for (size_type i = begin; i != end; ++i) {
            printf("CELL %u: %" PRIu8 "\n", (unsigned) i, memory[i]);
        }
        printf("\n");
    }

    void print_source_line(size_type pos) {
        /*if (pos < program.size()) {
            for (const auto &c : input[pos]) {
                std::cout << c << ' ';
            }
        }*/
        if (debug.find(pos) != debug.end()) {
            std::cout << source[debug[pos]] << std::endl << ">>> ";
        }
        else {
            std::cout << std::endl << ">>> ";
        }
        /*std::cout << std::endl << ">>> ";*/
    }

    void eval_n(size_type n, size_type &pos, uint32_t &uptime) {
        for (size_type i = 0; i != n && pos < program.size(); ++i) {
            uptime += 3 + calc_time(program[pos]);
            pos = operations.at(program[pos].name)(program[pos].addrs, pos);
        }
    }

    void run(void) {
        size_type pos = 0;
        uint32_t uptime = 0;
        std::regex regs_re(R"(\s*r\s*)"),
                   quit_re(R"(\s*q\s*)"),
                   mem_re(R"(\s*m\s+([0-9]+):([0-9]+)\s*)"),
                   steps_re(R"(\s*s\s+([0-9]+)\s*)"),
                   times_re(R"(\s*t\s*)");
        std::smatch match;
        std::string str;
        while (true) {
            print_source_line(pos);
            std::getline(std::cin, str);
            if (str.empty()) {
                eval_n(1, pos, uptime);
            }
            else if (std::regex_match(str, match, regs_re)) {
                print_regs();
            }
            else if (std::regex_match(str, match, times_re)) {
                print_time(pos, uptime);
            }
            else if (std::regex_match(str, match, steps_re)) {
                size_type n = std::stoull(match.str(1), nullptr, 10);
                eval_n(n, pos, uptime);
            }
            else if (std::regex_match(str, match, mem_re)) {
                size_type b = std::stoull(match.str(1), nullptr, 10),
                          n = std::stoull(match.str(2), nullptr, 10);
                print_mem_range(b, b + n);
            }
            else if (std::regex_match(str, match, quit_re)) {
                break;
            }
            else {
                std::cout << "Unknown command" << std::endl;
            }
        }
    }
}


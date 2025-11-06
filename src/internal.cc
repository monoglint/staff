#include "internal.hh"

/*

File file format

(tok) - name count
'identifier_names'
    (dynamic) NAME
(tok) - staff count
'staff entries'    
    (8 bit) NAME
    (8 bit) AVAILABILITY
    DELIMITER
(tok) - student count
'student entries'
    (8 bit) NAME
    (8 bit) A LUNCH
    (8 bit) B LUNCH
    DELIMITER

    '^ - yeah, thats dumb, we could combine them into a byte but we're on limmited time'

*/
bool save_state::save() const {
    std::ofstream file(file_name, std::ios::binary | std::ios::out);

    if (!file.is_open()) {
        return false;
    }

    std::stringstream buffer;

    buffer << (uint8_t)name_list.size() << TOK_DELIM;
    for (const std::string& name : name_list) {
        buffer << name << TOK_DELIM;
    }

    buffer << (uint8_t)staff_list.size() << TOK_DELIM;
    for (const staff_entry& entry : staff_list) {
        buffer << (uint8_t)entry.name_id << (uint8_t)entry.availability << TOK_DELIM;
    }

    buffer << (uint8_t)student_list.size() << TOK_DELIM;
    for (const student_entry& entry : student_list) {
        buffer << (uint8_t)entry.name_id << (uint8_t)entry.a_day_lunch << (uint8_t)entry.b_day_lunch << TOK_DELIM;
    }
    
    file.write(buffer.str().c_str(), buffer.str().size());

    return true;
};

inline bool operator==(const std::string& str, const char c) {
    if (str.size() != 1)
        return false;

    return str[0] == c;
}

inline bool operator!=(const std::string& str, const char c) {
    return !(str == c);
}

static bool parse_token_list(save_state& state, const std::vector<std::string>& token_list) {
    uint16_t ip = 0;

    // Work with identifiers
    uint8_t identifier_count = token_list[ip++][0];
    for (int i = 0; i < identifier_count; i++) {
        state.name_list.push_back(token_list[ip++]);
    }

    uint8_t staff_count = token_list[ip++][0];
    for (int i = 0; i < staff_count; i++) {
        const std::string& tok = token_list.at(ip++);

        uint8_t b_name = tok[0];
        uint8_t b_availability = tok[1];

        state.append_staff(staff_entry(b_name, (f_lunch_availability)b_availability));
    }

    uint8_t student_count = token_list[ip++][0];
    for (int i = 0; i < student_count; i++) {
        const std::string& tok = token_list[ip++];

        uint8_t b_name = tok[0];
        uint8_t b_a_lunch = tok[1];
        uint8_t b_b_lunch = tok[2];

        state.append_student(student_entry(b_name, (e_lunch_period)b_a_lunch, (e_lunch_period)b_b_lunch));
    }

    return true;
}

bool save_state::load() {
    std::ifstream file(file_name, std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        return false;
    }

    // Collect tokens into list
    char temp;
    std::stringstream buffer;
    std::vector<std::string> token_list;
    while (file.read(reinterpret_cast<char*>(&temp), 1)) {
        if (temp == TOK_DELIM) {
            token_list.push_back(buffer.str());
            buffer.str("");
            continue;
        }

        buffer << temp;
    }

    return parse_token_list(*this, token_list);
};
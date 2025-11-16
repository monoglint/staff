#pragma once

#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstdint>

#include "core.hh"

// Save states are a tiny bit fragile - limits are enforced in api.hh

constexpr uint8_t TOK_DELIM = '~'; // this is not a token, it delimits them

inline void remove_delim_in_string(std::string& str) {
    for (std::string::iterator it = str.begin(); it != str.end();) {
        if (*it == TOK_DELIM)
            it = str.erase(it);
        else
            ++it;
    }
}

enum class e_lunch_period : uint8_t {
    A,
    B,
    C,
    _MAX,
};

const uint8_t ADD_CONSTANT = 3;

using t_period_index_base = size_t;
enum class e_period_index : t_period_index_base {
    AA = 0, // A
    AB = 1, // B
    AC = 2, // C
    BA = 3, // A + ADD_CONSTANT
    BB = 4, // B + ADD_CONSTANT
    BC = 5, // C + ADD_CONSTANT
    _MAX,
};

using t_period_index_list = std::vector<e_period_index>;

enum class f_period_index : uint8_t {
    NONE = 0,
    AA = 1 << 0,
    AB = 1 << 1,
    AC = 1 << 2,
    BA = 1 << 3,
    BB = 1 << 4,
    BC = 1 << 5,
};

inline f_period_index operator|(f_period_index a, f_period_index b) {
    return (f_period_index)((uint8_t)a | (uint8_t)b);
}

inline f_period_index operator&(f_period_index a, f_period_index b) {
    return (f_period_index)((uint8_t)a & (uint8_t)b);
}

inline f_period_index operator~(f_period_index a) {
    return (f_period_index)(~(uint8_t)a);
}

using t_name_list = std::vector<std::string>;

using t_serialized_entry = uint32_t;
using t_serialized_entry_list = std::vector<t_serialized_entry>;

#define FM_B(la, str) if (flag_match(availability, f_period_index::la)) buffer << str;
inline std::string to_string(const f_period_index& availability) {
    std::stringstream buffer;

    FM_B(AA, "AA ")
    FM_B(AB, "AB ")
    FM_B(AC, "AC ")
    FM_B(BA, "BA ")
    FM_B(BB, "BB ")
    FM_B(BC, "BC ")

    if (buffer.str().length() == 0)
        buffer << "NO AVAILABILITY\n";

    return buffer.str();
}
#undef FM_B

inline std::string to_string(const e_lunch_period& period) {
    switch (period) {
        case e_lunch_period::A:
            return "A";
        case e_lunch_period::B:
            return "B";
        case e_lunch_period::C:
            return "C";
        default:
            return "INVALID";
    }
}

struct staff_entry {
    staff_entry(const std::string& name, const f_period_index availability = f_period_index::NONE)
        : name(name), availability(availability) {}

    staff_entry()
        : name("No Name"), availability(f_period_index::NONE) {}

    std::string name;
    f_period_index availability;
};

struct student_entry {
    student_entry(const std::string& name, const e_lunch_period a_day_lunch, const e_lunch_period b_day_lunch)
        : name(name), a_day_lunch(a_day_lunch), b_day_lunch(b_day_lunch) {}

    student_entry()
        : name("No Name"), a_day_lunch(e_lunch_period::A), b_day_lunch(e_lunch_period::A) {}

    std::string name;

    e_lunch_period a_day_lunch;
    e_lunch_period b_day_lunch;
};

using t_staff_list = std::vector<staff_entry>;
using t_student_list = std::vector<student_entry>;

using t_entry_id = size_t;
using t_entry_id_list = std::vector<t_entry_id>;

const t_entry_id ENTRY_MAX = _UI64_MAX;

struct save_state {
    save_state(const std::string& file_name, const t_staff_list& staff_list, const t_student_list& student_list)
        : file_name(file_name), staff_list(staff_list), student_list(student_list) {}

    save_state(const std::string& file_name)
        : file_name(file_name) {
        const bool success = load();

        if (!success) {
            std::cout << "First open - Generating new empty file.\n";
        }
    }

    std::string file_name;

    t_staff_list staff_list;
    t_student_list student_list;

    bool save() const;
    bool load();

    inline t_entry_id append_staff(staff_entry&& entry) {
        staff_list.push_back(std::move(entry));

        return staff_list.size() - 1;
    }

    inline t_entry_id append_student(student_entry&& entry) {
        student_list.push_back(std::move(entry));

        return student_list.size() - 1;
    }
};
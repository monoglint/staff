#include <iostream>
#include <limits>

#include "api.hh"

template <typename T>
static inline T in_t() {
    T in;
    std::cin >> in;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return in;
}

static inline std::string in_string() {
    std::string str;
    std::getline(std::cin, str);

    return str;
}

static inline std::string prompt_str(const std::string& text) {
    std::cout << text << ": ";
    return in_string();
}

template <typename T>
static inline T prompt_t(const std::string& text) {
    std::cout << text << ": ";
    return in_t<T>();
}

static inline void prompt_continue() {
    std::cout << "Press enter to continue.\n";
    std::cin.get(); 
}

static void add_staff(staff_session& active_session) {
    system("cls");
    std::string name = prompt_str("Please provide a name");

    int periods = prompt_t<int>("How many periods max is the staff willing to be on duty for?");

    while (periods > 6) {
        periods = prompt_t<int>("Please enter a value between 1 and 6.");
    }

    std::unordered_set<e_period_index> available_periods;

    for (t_period_index_base period = 0; period < (t_period_index_base)e_period_index::_MAX; period++) {
        system("cls");

        std::cout << "Enter period availability:\n\n";

        switch ((t_period_index_base)period) {
            case 0: std::cout << "A day\n\nA lunch: "; break;
            case 1: std::cout << "A day\n\nB lunch: "; break;
            case 2: std::cout << "A day\n\nC lunch: "; break;
            case 3: std::cout << "B day\n\nA lunch: "; break;
            case 4: std::cout << "B day\n\nB lunch: "; break;
            case 5: std::cout << "B day\n\nC lunch: "; break;
        }

        char in = prompt_t<char>("Does the teacher want to watch this period? (y/n)");

        while (in != 'y' && in != 'n') {
            in = prompt_t<char>("Please enter 'y' or 'n'");
        }

        if (in == 'y')
            available_periods.insert((e_period_index)period);
    }
    
    active_session.add_staff(name, periods, available_periods);

    std::cout << "Added staff.\n";
    prompt_continue();
}

e_lunch_period char_to_period(const char c) {
    switch (c) {
        case 'a':
            return e_lunch_period::A;
        case 'b':
            return e_lunch_period::B;
        case 'c':
            return e_lunch_period::C;
        default:
            break;
    }

    __builtin_unreachable();
}

static void add_student(staff_session& active_session) {
    system("cls");
    std::string name = prompt_str("Please provide a name");

    system("cls");
    char a_day_lunch = prompt_t<char>("Which lunch does this student have on a-day? (a, b, c)");

    while (a_day_lunch != 'a' && a_day_lunch != 'b' && a_day_lunch != 'c') {
        a_day_lunch = prompt_t<char>("Please enter 'a', 'b', or 'c'.");
    }

    system("cls");
    char b_day_lunch = prompt_t<char>("Which lunch does this student have on a-day? (a, b, c)");

    while (b_day_lunch != 'a' && b_day_lunch != 'b' && b_day_lunch != 'c') {
        b_day_lunch = prompt_t<char>("Please enter 'a', 'b', or 'c'.");
    }

    system("cls");

    active_session.add_student(name, char_to_period(a_day_lunch), char_to_period(b_day_lunch));

    std::cout << "Added student.\n";
    prompt_continue();
}

template <e_entry_type TYPE>
static void remove(staff_session& active_session) {
    system("cls");
    std::string name = prompt_str("Please provide a name to remove");

    staff_session::e_remove_code result = active_session.remove<TYPE>(name);

    switch (result) {
        case staff_session::e_remove_code::SUCCESS:
            std::cout << "Removed \"" << name << "\"\n";
            break;
        case staff_session::e_remove_code::NO_MATCHING_NAME:
            std::cout << "Nobody was removed.\n";
            break;
    }

    prompt_continue();
}

static void make_schedule(staff_session& active_session) {
    system("cls");
    std::cout << active_session.print_distribution_export().str();

    prompt_continue();
}

int main() {
    staff_session active_session("staffsave.ls");

    while (true) {
        system("cls");

        std::cout << active_session.print_state().str();

        int input = prompt_t<int>("(1) Add Staff\n(2) Add Student\n(3) Remove Staff\n(4) Remove Student\n(5) Make Schedule\n(6) Reload\n(Etc) Quit\n");
        
        switch (input) {
            case 1:
                add_staff(active_session);
                break;
            case 2:
                add_student(active_session);
                break;
            case 3:
                remove<e_entry_type::STAFF>(active_session);
                break;
            case 4:
                remove<e_entry_type::STUDENT>(active_session);
                break;
            case 5:
                make_schedule(active_session);
                break;
            case 6:
                break;
            default:
                goto exit;
        }
    }

    exit:

    return 0;
}
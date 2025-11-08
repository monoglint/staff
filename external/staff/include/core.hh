#pragma once

template <typename T>
inline bool flag_match(const T& flag_set, const T& flag) {
    return (flag_set & flag) == flag;
}

template <typename ENUM, typename FLAGSET>
inline FLAGSET enum_to_flag(const ENUM e) {
    return (FLAGSET)(1 << (int)e);
}
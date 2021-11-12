#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H


#include <string.h>
#include <cstdio>
#include <cstdlib>


class Logger {
    FILE *fileptr;
    
    char  *last_announcer;
    size_t last_announcer_len;

    char  *last_code;
    size_t last_code_len;

    size_t max_code_len;
    size_t max_announcer_len;

    bool to_print_announcer;
    bool to_print_code;

    int reset_max_lens_counter;
    int tick_timer;

    int log_level;
    int verb_level;

    int page_cnt;

    void update_announcer(const char* announcer);
    void update_code(const char *code);

    void reset_lasts();

    void print_n_spaces(size_t n);

    void print_nullptr_passed_error() const;

    void tick();

    void _log(bool to_ignore_log_level, const char* code, const char* announcer, const char *message, va_list arglist);

public:
    int paging_mode;

    enum class Level {
        none = 0,
        error = 2,
        warning = 4,
        info = 6,
        debug = 10,
    };

    Logger(FILE *fileptr = stdout, int log_level=5, int reset_max_lens_counter = 50);

    void log(const char* code, const char* announcer, const char *message, ...); // normal logging
    void log(int override_log_level, const char* code, const char* announcer, const char *message, ...);
    void logr(const char* code, const char* announcer, const char *message, ...); // reset maxlens and lasts
    
    void error   (const char* announcer, const char *message, ...);
    void ERROR   (const char* announcer, const char *message, ...);
    void info    (const char* announcer, const char *message, ...);
    void warning (const char* announcer, const char *message, ...);
    void doubt   (const char* announcer, const char *message, ...);

    void n();
    void page_cut(const char *page_name = nullptr, int page_len = 80, char symb = '=');

    inline void resets() {
        reset_lasts();
        reset_max_lens();
        tick_timer = 0;
    }

    int  get_log_level() const;
    void set_log_level(int log_level_);
    void set_log_level(Logger::Level log_level_);

    int  get_verb_level() const;
    void set_verb_level(int verb_level_);
    void set_verb_level(Logger::Level verb_level_);

    void reset_max_lens();
};

extern Logger logger;

class LogLevel {
    Logger &logger;
    int prev_log_level;
    int prev_verb_level;

public:
    LogLevel(Logger &logger, int log_level = -1, int verb_level = -1);
    ~LogLevel();
};

#endif // UTIL_LOGGER_H

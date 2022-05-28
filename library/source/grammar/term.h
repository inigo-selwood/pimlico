#ifndef _PEG_TERM_H
#define _PEG_TERM_H

typedef uint8_t PEG_Bool;

typedef struct PEG_Term {

    union {
        const char *text;
        struct PEG_Term *term;
        struct PEG_Term **terms;
    } value;

    enum {
        PEG_CONSTANT  = 1,
        PEG_RANGE     = 2,
        PEG_REFERENCE = 3,
        PEG_SEQEUNCE  = 4,
        PEG_SET       = 5,
    } type;

    uint8_t size;

} PEG_Term;

// Create a constant term
PEG_Term *PEG_termCreateConstant(const char *value);

// Create a range term
PEG_Term *PEG_termCreateRange(const char lower, const char upper);

// Create a reference term
PEG_Term *PEG_termCreateReference(PEG_Term *value);

// Create a sequence term
PEG_Term *PEG_termCreateSequence(PEG_Term **terms, const uint8_t termCount);

// Create a set term
PEG_Term *PEG_termCreateSet(const char *values);

// Free the memory held by a term
void PEG_termDestroy(PEG_Term *term);

#endif // _PEG_TERM_H
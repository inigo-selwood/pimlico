#include "term.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a constant term
PEG_Term *PEG_termCreateConstant(const char *value) {
    PEG_Term *constant = (PEG_Term *)malloc(sizeof(PEG_Term));

    constant->type = PEG_CONSTANT;
    constant->value.text = strdup(value);
    constant->size = strlen(value);

    return constant;
}

// Create a range term
PEG_Term *PEG_termCreateRange(const char lower, const char upper) {
    PEG_Term *range = (PEG_Term *)malloc(sizeof(PEG_Term));

    range->type = PEG_RANGE;
    range->size = 2;

    range->value.text = (char *)malloc(sizeof(char) * 2);
    char temporary[2] = {lower, upper};
    range->value.text = strdup(temporary);

    return range;
}

// Create a reference term
PEG_Term *PEG_termCreateReference(PEG_Term *value) {
    PEG_Term *reference = (PEG_Term *)malloc(sizeof(PEG_Term));

    reference->type = PEG_REFERENCE;
    reference->size = 1;
    reference->value.term = value;
    
    return reference;
}

// Create a sequence term
PEG_Term *PEG_termCreateSequence(PEG_Term **terms, const uint8_t termCount) {
    PEG_Term *sequence = (PEG_Term *)malloc(sizeof(PEG_Term));

    sequence->type = PEG_SEQEUNCE;
    sequence->size = termCount;

    const size_t size = sizeof(PEG_Term *) * termCount;
    sequence->value.terms = (PEG_Term **)malloc(size);

    for(uint8_t index = 0; index < termCount; index += 1)
        sequence->value.terms[index] = terms[index];

    return sequence;
}

// Create a set term
PEG_Term *PEG_termCreateSet(const char *values) {
    PEG_Term *set = (PEG_Term *)malloc(sizeof(PEG_Term));

    set->type = PEG_SET;
    set->value.text = strdup(values);
    set->size = strlen(values);

    return set;
}

// Free the memory held by a term
void PEG_termDestroy(PEG_Term *term) {
    assert(term);

    switch(term->type) {
        case PEG_CONSTANT:
        case PEG_RANGE:
        case PEG_SET:
            free((char *)term->value.text);
            term->value.text = NULL;
            break;
        
        case PEG_SEQEUNCE:
            free(term->value.terms);
            term->value.terms = NULL;
            break;
        
        default:
            break;
    }

    free(term);
}

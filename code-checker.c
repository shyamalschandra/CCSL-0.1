/**
 * @file code-checker.c
 * @brief Simple command-line utility to check code quality using CCSL metrics
 * @author Shyamal Chandra (C) 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* Maximum file size to read */
#define MAX_FILE_SIZE 1048576 /* 1MB */

/* Metric types */
typedef enum {
    METRIC_IMPACT = 0,
    METRIC_SIMPLICITY = 1,
    METRIC_CLEANNESS = 2,
    METRIC_COMMENT = 3,
    METRIC_CREDITABILITY = 4,
    METRIC_NOVELTY = 5,
    METRIC_COUNT = 6
} metric_type_t;

/* Metric scores */
typedef struct {
    double scores[METRIC_COUNT];
    char *rationales[METRIC_COUNT];
} metric_scores_t;

/* Function prototypes */
char* read_file(const char *filename);
metric_scores_t evaluate_code(const char *code);
double calculate_impact(const char *code);
double calculate_simplicity(const char *code);
double calculate_cleanness(const char *code);
double calculate_comment(const char *code);
double calculate_creditability(const char *code);
double calculate_novelty(const char *code);
void print_results(const metric_scores_t *scores);
void free_scores(metric_scores_t *scores);
char* generate_rationale(metric_type_t type, double score);

int main(int argc, char *argv[]) {
    /* Check arguments */
    if (argc < 2) {
        printf("Usage: %s <file1> [file2] ...\n", argv[0]);
        return 1;
    }

    /* Process each file */
    for (int i = 1; i < argc; i++) {
        printf("\nAnalyzing file: %s\n", argv[i]);
        printf("====================\n");
        
        /* Read the file */
        char *code = read_file(argv[i]);
        if (!code) {
            printf("Error: Failed to read file '%s'\n", argv[i]);
            continue;
        }
        
        /* Evaluate code */
        metric_scores_t scores = evaluate_code(code);
        
        /* Print results */
        print_results(&scores);
        
        /* Clean up */
        free_scores(&scores);
        free(code);
    }
    
    return 0;
}

/* Read a file into memory */
char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;
    
    /* Get file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    /* Limit file size */
    if (size > MAX_FILE_SIZE) {
        fprintf(stderr, "File too large: %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    /* Allocate memory for file content */
    char *buffer = (char *) malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    /* Read file content */
    size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    
    fclose(file);
    return buffer;
}

/* Evaluate code using all metrics */
metric_scores_t evaluate_code(const char *code) {
    metric_scores_t scores;
    
    /* Calculate scores for each metric */
    scores.scores[METRIC_IMPACT] = calculate_impact(code);
    scores.scores[METRIC_SIMPLICITY] = calculate_simplicity(code);
    scores.scores[METRIC_CLEANNESS] = calculate_cleanness(code);
    scores.scores[METRIC_COMMENT] = calculate_comment(code);
    scores.scores[METRIC_CREDITABILITY] = calculate_creditability(code);
    scores.scores[METRIC_NOVELTY] = calculate_novelty(code);
    
    /* Generate rationales */
    for (int i = 0; i < METRIC_COUNT; i++) {
        scores.rationales[i] = generate_rationale((metric_type_t)i, scores.scores[i]);
    }
    
    return scores;
}

/* Calculate Impact metric (0.0 - 1.0) */
double calculate_impact(const char *code) {
    double score = 0.0;
    int total_lines = 0;
    int function_calls = 0;
    int control_statements = 0;
    
    /* Count lines */
    const char *ptr = code;
    while (*ptr) {
        if (*ptr == '\n') total_lines++;
        ptr++;
    }
    
    /* Count function calls */
    ptr = code;
    while ((ptr = strstr(ptr, "("))) {
        /* Check if it's likely a function call */
        if (ptr > code && isalpha(*(ptr-1))) {
            function_calls++;
        }
        ptr++;
    }
    
    /* Count control statements */
    const char *keywords[] = {"if", "for", "while", "switch", "return"};
    for (int i = 0; i < 5; i++) {
        const char *p = code;
        while ((p = strstr(p, keywords[i]))) {
            /* Check if it's a whole word */
            if ((p == code || !isalnum(*(p-1))) && 
                !isalnum(*(p + strlen(keywords[i])))) {
                control_statements++;
            }
            p += strlen(keywords[i]);
        }
    }
    
    /* Calculate score */
    if (total_lines > 0) {
        double density = (double)(function_calls + control_statements) / total_lines;
        score = density > 0.5 ? 0.5 + (density - 0.5) * 0.5 : density;
        
        /* Cap at 1.0 */
        if (score > 1.0) score = 1.0;
    }
    
    return score;
}

/* Calculate Simplicity metric (0.0 - 1.0) */
double calculate_simplicity(const char *code) {
    double score = 0.0;
    int total_lines = 0;
    int long_lines = 0;
    int nesting_depth = 0;
    int max_nesting_depth = 0;
    
    /* Count lines and long lines */
    const char *line_start = code;
    while (*line_start) {
        /* Find end of line */
        const char *line_end = strchr(line_start, '\n');
        if (!line_end) line_end = line_start + strlen(line_start);
        
        int line_length = (int)(line_end - line_start);
        total_lines++;
        
        /* Check if line is long (>80 chars) */
        if (line_length > 80) {
            long_lines++;
        }
        
        /* Track nesting depth */
        for (const char *p = line_start; p < line_end; p++) {
            if (*p == '{') {
                nesting_depth++;
                if (nesting_depth > max_nesting_depth) {
                    max_nesting_depth = nesting_depth;
                }
            } else if (*p == '}') {
                nesting_depth--;
            }
        }
        
        /* Move to next line */
        line_start = line_end;
        if (*line_start == '\n') line_start++;
    }
    
    /* Calculate score */
    double long_line_penalty = total_lines > 0 ? (double)long_lines / total_lines : 0;
    double nesting_penalty = max_nesting_depth > 3 ? (max_nesting_depth - 3) * 0.1 : 0;
    
    score = 1.0 - long_line_penalty - nesting_penalty;
    
    /* Ensure score is in range */
    if (score < 0.0) score = 0.0;
    if (score > 1.0) score = 1.0;
    
    return score;
}

/* Calculate Cleanness metric (0.0 - 1.0) */
double calculate_cleanness(const char *code) {
    double score = 0.0;
    int total_lines = 0;
    int indented_lines = 0;
    int consistent_spacing = 0;
    int inconsistent_spacing = 0;
    
    /* Count lines and check indentation */
    const char *line_start = code;
    while (*line_start) {
        /* Find end of line */
        const char *line_end = strchr(line_start, '\n');
        if (!line_end) line_end = line_start + strlen(line_start);
        
        total_lines++;
        
        /* Check if line is indented correctly */
        int spaces = 0;
        for (const char *p = line_start; p < line_end && isspace(*p); p++) {
            if (*p == ' ') spaces++;
        }
        
        /* Consider lines with multiple of 2 or 4 spaces as properly indented */
        if (spaces > 0 && (spaces % 2 == 0 || spaces % 4 == 0)) {
            indented_lines++;
        }
        
        /* Check operator spacing */
        for (const char *p = line_start; p < line_end; p++) {
            if (*p == '=' || *p == '+' || *p == '-' || *p == '*' || *p == '/') {
                if ((p > line_start && *(p-1) == ' ' && *(p+1) == ' ') ||
                    (*(p+1) != ' ' && *(p-1) != ' ')) {
                    consistent_spacing++;
                } else {
                    inconsistent_spacing++;
                }
            }
        }
        
        /* Move to next line */
        line_start = line_end;
        if (*line_start == '\n') line_start++;
    }
    
    /* Calculate score */
    double indentation_score = total_lines > 0 ? (double)indented_lines / total_lines : 0;
    double spacing_score = (consistent_spacing + inconsistent_spacing) > 0 ? 
                          (double)consistent_spacing / (consistent_spacing + inconsistent_spacing) : 0.5;
    
    score = 0.7 * indentation_score + 0.3 * spacing_score;
    
    /* Ensure score is in range */
    if (score < 0.0) score = 0.0;
    if (score > 1.0) score = 1.0;
    
    return score;
}

/* Calculate Comment metric (0.0 - 1.0) */
double calculate_comment(const char *code) {
    double score = 0.0;
    int total_lines = 0;
    int comment_lines = 0;
    int doc_comment_lines = 0;
    
    /* Count lines and comments */
    const char *line_start = code;
    int in_comment = 0;  /* 1 for // comments, 2 for /* comments */
    
    while (*line_start) {
        /* Find end of line */
        const char *line_end = strchr(line_start, '\n');
        if (!line_end) line_end = line_start + strlen(line_start);
        
        total_lines++;
        
        /* Skip leading whitespace */
        const char *p = line_start;
        while (p < line_end && isspace(*p)) p++;
        
        /* Check for comments */
        if (in_comment == 2) {
            /* Inside a multi-line comment */
            comment_lines++;
            
            /* Check if this line has a */ closing the comment */
            const char *end_comment = strstr(line_start, "*/");
            if (end_comment && end_comment < line_end) {
                in_comment = 0;
            }
            
            /* Check for doc comment features */
            if (strstr(line_start, "@param") || strstr(line_start, "@return") ||
                strstr(line_start, "@brief") || strstr(line_start, "@see")) {
                doc_comment_lines++;
            }
        } else {
            /* Check for single-line comments */
            const char *comment_start = strstr(p, "//");
            if (comment_start && comment_start < line_end) {
                comment_lines++;
            }
            
            /* Check for multi-line comment start */
            comment_start = strstr(p, "/*");
            if (comment_start && comment_start < line_end) {
                comment_lines++;
                in_comment = 2;
                
                /* Check for doc comment features */
                if (*(comment_start+2) == '*') {
                    /* Doc comment */
                    doc_comment_lines++;
                }
                
                /* Check if the comment ends on the same line */
                const char *end_comment = strstr(comment_start + 2, "*/");
                if (end_comment && end_comment < line_end) {
                    in_comment = 0;
                }
            }
        }
        
        /* Move to next line */
        line_start = line_end;
        if (*line_start == '\n') line_start++;
    }
    
    /* Calculate score */
    double comment_ratio = total_lines > 0 ? (double)comment_lines / total_lines : 0;
    double doc_comment_ratio = comment_lines > 0 ? (double)doc_comment_lines / comment_lines : 0;
    
    /* Ideal comment ratio is around 0.2-0.3 (too many or too few comments are not good) */
    double comment_score = comment_ratio <= 0.3 ? comment_ratio / 0.3 : (1.0 - comment_ratio) / 0.7;
    
    score = 0.6 * comment_score + 0.4 * doc_comment_ratio;
    
    /* Ensure score is in range */
    if (score < 0.0) score = 0.0;
    if (score > 1.0) score = 1.0;
    
    return score;
}

/* Calculate Creditability metric (0.0 - 1.0) */
double calculate_creditability(const char *code) {
    double score = 0.0;
    int total_lines = 0;
    int has_author = 0;
    int has_license = 0;
    int has_version = 0;
    int has_references = 0;
    int has_tests = 0;
    
    /* Count lines and check creditability markers */
    const char *ptr = code;
    total_lines = 0;
    while (*ptr) {
        if (*ptr == '\n') total_lines++;
        ptr++;
    }
    
    /* Check for author information */
    if (strstr(code, "@author") || strstr(code, "Author:") || 
        strstr(code, "COPYRIGHT") || strstr(code, "Copyright") || 
        strstr(code, "(c)") || strstr(code, "(C)")) {
        has_author = 1;
    }
    
    /* Check for license information */
    if (strstr(code, "License") || strstr(code, "LICENSE") || 
        strstr(code, "MIT") || strstr(code, "GPL") || 
        strstr(code, "Apache") || strstr(code, "CCSL")) {
        has_license = 1;
    }
    
    /* Check for version information */
    if (strstr(code, "@version") || strstr(code, "Version:") || 
        strstr(code, "v1.") || strstr(code, "v0.")) {
        has_version = 1;
    }
    
    /* Check for references */
    if (strstr(code, "@see") || strstr(code, "http://") || 
        strstr(code, "https://") || strstr(code, "www.")) {
        has_references = 1;
    }
    
    /* Check for tests */
    if (strstr(code, "test") || strstr(code, "Test") || 
        strstr(code, "assert") || strstr(code, "Assert") ||
        strstr(code, "expect") || strstr(code, "should")) {
        has_tests = 1;
    }
    
    /* Calculate score */
    score = 0.2 * has_author + 0.2 * has_license + 0.2 * has_version + 
            0.2 * has_references + 0.2 * has_tests;
    
    return score;
}

/* Calculate Novelty metric (0.0 - 1.0) */
double calculate_novelty(const char *code) {
    double score = 0.0;
    int has_templates = 0;
    int has_lambda = 0;
    int has_algorithms = 0;
    int has_concepts = 0;
    int has_meta = 0;
    int has_design_patterns = 0;
    
    /* Check for templates */
    if (strstr(code, "template") || strstr(code, "<typename") || 
        strstr(code, "<class")) {
        has_templates = 1;
    }
    
    /* Check for lambda expressions */
    if (strstr(code, "[]") || strstr(code, "=>") || 
        strstr(code, "->") || strstr(code, "auto")) {
        has_lambda = 1;
    }
    
    /* Check for advanced algorithms */
    if (strstr(code, "algorithm") || strstr(code, "transform") || 
        strstr(code, "accumulate") || strstr(code, "fold") ||
        strstr(code, "recursive")) {
        has_algorithms = 1;
    }
    
    /* Check for concepts (C++20) */
    if (strstr(code, "concept") || strstr(code, "requires") || 
        strstr(code, "std::same_as") || strstr(code, "std::convertible_to")) {
        has_concepts = 1;
    }
    
    /* Check for metaprogramming */
    if (strstr(code, "std::enable_if") || strstr(code, "std::conditional") || 
        strstr(code, "constexpr") || strstr(code, "if constexpr")) {
        has_meta = 1;
    }
    
    /* Check for design patterns */
    if (strstr(code, "Factory") || strstr(code, "Singleton") || 
        strstr(code, "Observer") || strstr(code, "Strategy") ||
        strstr(code, "Visitor") || strstr(code, "Adapter")) {
        has_design_patterns = 1;
    }
    
    /* Calculate score */
    score = 0.15 * has_templates + 0.15 * has_lambda + 0.20 * has_algorithms + 
            0.2 * has_concepts + 0.15 * has_meta + 0.15 * has_design_patterns;
    
    return score;
}

/* Print evaluation results */
void print_results(const metric_scores_t *scores) {
    const char *metric_names[] = {
        "Impact", "Simplicity", "Cleanness", 
        "Comment", "Creditability", "Novelty"
    };
    
    printf("CCSL Metric Evaluation Results:\n");
    printf("-------------------------------\n");
    
    double total_score = 0.0;
    for (int i = 0; i < METRIC_COUNT; i++) {
        printf("%s: %.2f\n", metric_names[i], scores->scores[i]);
        printf("  %s\n", scores->rationales[i]);
        total_score += scores->scores[i];
    }
    
    double average_score = total_score / METRIC_COUNT;
    printf("\nOverall Credit Score: %.2f / 1.00\n", average_score);
    
    /* Qualitative assessment */
    printf("Assessment: ");
    if (average_score >= 0.9) {
        printf("Excellent\n");
    } else if (average_score >= 0.8) {
        printf("Very Good\n");
    } else if (average_score >= 0.7) {
        printf("Good\n");
    } else if (average_score >= 0.6) {
        printf("Above Average\n");
    } else if (average_score >= 0.5) {
        printf("Average\n");
    } else if (average_score >= 0.4) {
        printf("Below Average\n");
    } else if (average_score >= 0.3) {
        printf("Poor\n");
    } else {
        printf("Very Poor\n");
    }
}

/* Generate a rationale for a metric score */
char* generate_rationale(metric_type_t type, double score) {
    char *rationale = malloc(256);
    if (!rationale) return NULL;
    
    const char *high_rationales[] = {
        "High functional significance with good control flow",
        "Clean and straightforward implementation, easy to understand",
        "Well-formatted with consistent style and spacing",
        "Well-documented with helpful comments at appropriate density",
        "Includes comprehensive attribution and references",
        "Uses advanced language features and design patterns"
    };
    
    const char *medium_rationales[] = {
        "Moderate functional significance with acceptable control flow",
        "Reasonably clear implementation with some complexity",
        "Generally consistent formatting with minor inconsistencies",
        "Contains some useful comments but could be improved",
        "Includes some attribution information",
        "Uses some modern language features"
    };
    
    const char *low_rationales[] = {
        "Limited functional significance with unclear control flow",
        "Overly complex or confusing implementation",
        "Inconsistent formatting and style",
        "Lacks sufficient commenting or has too many unhelpful comments",
        "Missing attribution and references",
        "Uses only basic language features"
    };
    
    const char **rationales;
    if (score >= 0.7) {
        rationales = high_rationales;
    } else if (score >= 0.4) {
        rationales = medium_rationales;
    } else {
        rationales = low_rationales;
    }
    
    snprintf(rationale, 256, "%s", rationales[type]);
    return rationale;
}

/* Free resources used by metric scores */
void free_scores(metric_scores_t *scores) {
    for (int i = 0; i < METRIC_COUNT; i++) {
        free(scores->rationales[i]);
    }
}
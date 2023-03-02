#include "Data_Aggregator.h"

int main() {
    Data_Aggregator I;
    I.process_header();
    I.process_data();
    I.process_query();
    I.produce_output();
}

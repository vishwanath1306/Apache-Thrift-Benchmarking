service Compute {
    void computed_value(1: i32 row, 2: i32 column)
    string compute_list(1: list<list<list<double>>> values)
}
namespace cpp AsyncService

service AsyncService{

    oneway void compute_result(1: i64 num_1, 2: i64 num_2);

}
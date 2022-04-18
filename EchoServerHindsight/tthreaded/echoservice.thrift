namespace cpp echoservice


struct TraceContext {
    1: i64 req_id;
    2: string baggage;
}

service EchoService {
    void call (
        1: TraceContext req,
        2: string message
    )
}
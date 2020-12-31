package main

import (
	"fmt"

	"./gen-go/hello"

	// "hellothrift/hello"

	"git.apache.org/thrift.git/lib/go/thrift"
)

func RunClient(transportFactory thrift.TTransportFactory, protocolFactory thrift.TProtocolFactory, addr string) error {
	socket, err := thrift.NewTSocket(addr)
	if err != nil {
		return err
	}

	transport, err := transportFactory.GetTransport(socket)
	defer transport.Close()
	if err := transport.Open(); err != nil {
		return err
	}

	client := hello.NewHelloWorldClientFactory(transport, transportFactory)
	request := hello.NewHelloWorldRequest()
	response, err := client.Hello(request)
	if err != nil {
		return err
	}
	fmt.Println(response)

	return nil
}

func main() {
	transportFactory := thrift.NewTFramedTransportFactory(thrift.NewTTransportFactory())
	protocolFactory := thrift.NewTBinaryProtocolFactoryDefault()
	addr := "localhost:3062"
	err := RunClient(transportFactory, protocolFactory, addr)
	if err != nil {
		panic(err)
	}
}

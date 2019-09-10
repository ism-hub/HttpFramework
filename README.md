# Http Framework
This framework will be reworked in the next week or two so this file is less concerned with how to use the current version and more concerned about the big picture and the changes that will come.
If you still interested in using it you can check out the detailed example in the examples folder. 

A dispatcher which dispatches requests to user defined controllers (Based on Spring - MVC Framework).
Helps the user focus more on the domain logic and less with the HTTP overhead.
Can run on the ESP8266.

# Overview 
The main purpose of the framework is to let the user concentrate on the domain logic, while having a modular/reusable/testable code.
We do this by imposing as little as possible constraints on his domain and generating code for the user using attributes.

### Simplified Sequence diagram
for each execution chain we ask if it can handle the HTTPRequest, execution-chain has one controller (were the domain logic resides) and can have many interceptors (it appears that it has only one in the diagram cause its simplified version)
![Simplified Sequence diagram](https://i.ibb.co/ChYTrMp/image.png)

# Why The Rework
- The user has to do a lot of overhead in order to create a simple controller that returns an object (and the framework then serialize it to json and returns it as the response to the client).
- Moreover, the domain model should follow the visitor design pattern meaning it should inherit from an abstract acceptable class and implements the accept virtual method.

So, we have a lot of overhead and we are imposing constraints on the domain classes.

# What We Hope To Achieve After The Rework
- Minimal overhead, the user should focus on the domain logic, common use-cases (like returning a json of a domain model) should be straight forward with no overhead at all.
- No constraints on the domain model.
- Possibility to add 'schemes' (possibility to add more 'common use-cases').

### Example For The API We Want
The framework will create GardenController for us and inject all the necessary services (if any).
On a GET request with an URL of "/plants/:id" we will call 'plantRequest' method in the GardenController.
Because it is a 'JSONController' we will serialize the Plant object into json and send it in the request body.
```C++
[[JSONController]]
class GardenController {
    shared_ptr<ServiceWeNeed> _someService;
public:
    //service will be injected for us (we can ask for as many services as we want).
    GardenController(shared_ptr<ServiceWeWant> someService)  : _someService(someService) {}

    [[GET, "/plants/:id"]]
    const Plant& plantRequest(int id){
        //...
        return plant;
    }
}
```

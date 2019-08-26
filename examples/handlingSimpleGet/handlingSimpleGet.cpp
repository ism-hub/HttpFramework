/**
 * This example is everything you need to do in order for the dispatcher to handle requests to fetch data in Json format (REST style) with the framework:
 *  + Add the visitor design-pattern to your model.
 *  + Create the framework controllers.
 *  + Json Serializer for the model.
 *  + Serialization visitor.
 *  + Create the json interceptor (only calling a constructor)
 *  + Create the execution chain (only calling a constructor)
 *  + Create the dispatcher and add it the chains we just created (only calling a constructor).
 */

#include <iostream>
#include <memory>
#include <HttpFramework.hpp>

//************************************ Model ************************************
class Car;
class Store;
//******** Visitor design-pattern ********
class IDealershipVisitor {
public:
      virtual std::shared_ptr<void> visit(Car& car) = 0;
      virtual std::shared_ptr<void> visit(Store& store) = 0;
};

class IDealershipAcceptable {
public:
    virtual std::shared_ptr<void> accept(IDealershipVisitor& visitor) = 0;
};
//******** Model ********
class Car : public IDealershipAcceptable {
public:
    std::string brand;

    Car(const char* carBrand) : brand(carBrand) {}

    std::shared_ptr<void> accept(IDealershipVisitor& visitor) override {
         return visitor.visit(*this);
    }
};

class Store : public IDealershipAcceptable { 
public:
    std::string address;

    Store(const char* storeAddress) : address(storeAddress){}

    std::shared_ptr<void> accept(IDealershipVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

class CarDealership {
public:
    std::vector<Car> cars;
    std::vector<Store> stores;

    CarDealership() : cars{Car{"Ford"}, Car{"Toyota"}}, stores{Store{"Roses 9"}, Store{"Iris 12"}} {}
};

//********************************* serialization *********************************
// **** incomplete serialization service ****
class DelearshipSerializer {
public:
    void serialize(const Car& car, std::string& serialized) {
        serialized.append("{\"brand\":\"");
        serialized.append(car.brand);
        serialized.append("\"}");
    }
    
    void serialize(const Store& store, std::string& serialized) {
        serialized.append("{\"address\":\"");
        serialized.append(store.address);
        serialized.append("\"}");
    }
};

//**** serialization visitor ****
class DelearshipSerializerVisitor : public IDealershipVisitor {
    Http::SerializationVisitor<DelearshipSerializer, std::string> serVisitor; 
public:
    DelearshipSerializerVisitor() : serVisitor(std::make_shared<DelearshipSerializer>()){}

    std::shared_ptr<void> visit(Car& car) override {
        return serVisitor.visit(car);
    }
    std::shared_ptr<void> visit(Store& store) override {
        return serVisitor.visit(store);
    }
};

//*************************** HTTP framework constructs ***************************
class CarController : public Http::IController<IDealershipAcceptable, std::string> {
protected:
    CarDealership& _dealership;

public:
    CarController(CarDealership& dealership) : _dealership(dealership) {}

	virtual bool canHandle(Http::HttpServletRequest<std::string>& req) override {
        return req.urlTokens[0] == std::string("Cars"); 
    }
	virtual std::shared_ptr<IDealershipAcceptable> handle(Http::HttpServletRequest<std::string>& req, Http::HttpServletResponse<std::string>& response) override {
        return std::make_shared<Car>(_dealership.cars[req.urlTokens[1].stoi()]);
    }

};

class StoreController : public Http::IController<IDealershipAcceptable, std::string> {
protected:
    CarDealership& _dealership;

public:
    StoreController(CarDealership& dealership) : _dealership(dealership) {}

	virtual bool canHandle(Http::HttpServletRequest<std::string>& req) override {
        return req.urlTokens[0] == std::string("Stores"); 
    }
	virtual std::shared_ptr<IDealershipAcceptable> handle(Http::HttpServletRequest<std::string>& req, Http::HttpServletResponse<std::string>& response) override {
        return std::make_shared<Store>(_dealership.stores[req.urlTokens[1].stoi()]);
    }
};

int main(){
    //creating the model
    CarDealership dealership;

    //creating the execution chains (for car and store)
    auto jsonInterceptor = std::make_shared<Http::SerializationInterceptor<DelearshipSerializerVisitor, IDealershipAcceptable, std::string>>(std::make_shared<DelearshipSerializerVisitor>());

    auto carController = std::make_shared<CarController>(dealership);
    auto storeController = std::make_shared<StoreController>(dealership);

    auto carExecutionchain = std::make_shared<Http::HandlerExecutionChain2<IDealershipAcceptable, std::string>>(carController);
    auto storeExecutionchain = std::make_shared<Http::HandlerExecutionChain2<IDealershipAcceptable, std::string>>(storeController);

    carExecutionchain->addInterceptor(jsonInterceptor);
    storeExecutionchain->addInterceptor(jsonInterceptor);

    //creating the dispatcher and handling messages 
    Http::DispatcherServlet<std::string> dispatcher{carExecutionchain, storeExecutionchain};

    //requesting and getting car
    Http::HttpServletRequest<std::string> reqCar(""/*body*/, Http::HTTPMethod::HTTP_GET, "/Cars/0/"/*url*/);

    auto resCar = dispatcher.dispatch(reqCar);

    std::cout << "Car request response: " << std::endl;
    std::cout << "content: " << resCar->content << std::endl;
    std::cout << "content_type: " << resCar->content_type << std::endl;
    std::cout << "_httpCode: " << (int)resCar->_httpCode << std::endl;
    std::cout << std::endl;

    //requesting and getting store
    Http::HttpServletRequest<std::string> reqStore(""/*body*/, Http::HTTPMethod::HTTP_GET, "/Stores/1/"/*url*/);

    auto resStore = dispatcher.dispatch(reqStore);

    std::cout << "Store request response: " << std::endl;
    std::cout << "content: " << resStore->content << std::endl;
    std::cout << "content_type: " << resStore->content_type << std::endl;
    std::cout << "_httpCode: " << (int)resStore->_httpCode << std::endl;
    std::cout << std::endl;

    return 0;
}
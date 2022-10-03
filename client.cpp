#include <string>
#include <iostream>
#include <vector>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include <nlohmann/json.hpp>

using namespace std::chrono_literals;

int main() {
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t subscriber{context, zmq::socket_type::sub};
//	subscriber.set(zmq::sockopt::rcvhwm, 1);
	subscriber.set(zmq::sockopt::conflate, 1);

    subscriber.connect("tcp://localhost:5555");

    const std::string filter = "";
    subscriber.set(zmq::sockopt::subscribe, filter);

    std::vector<zmq::message_t> msgs;

    cv::Mat currentImage;

    for (auto request_num = 0; request_num < 100; ++request_num) {
	    std::chrono::steady_clock::time_point ts1 = std::chrono::steady_clock::now();





		zmq::message_t msg;
	    subscriber.recv(msg, zmq::recv_flags::none);

        cv::Mat foo(1, msg.size(), CV_8UC1, (void *) msg.data());
        cv::Mat decoded = cv::imdecode(foo, cv::IMREAD_COLOR);

	    std::chrono::steady_clock::time_point ts2 = std::chrono::steady_clock::now();

	    std::cout << "Single subscribe msg time: " << std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(
			    ts2 - ts1
	    ).count()) << std::endl;


//        cv::imshow("Image", decoded);
//        cv::waitKey(1);

//        std::this_thread::sleep_for(1s);
    }

    return 0;
}

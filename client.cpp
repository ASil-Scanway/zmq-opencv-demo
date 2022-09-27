#include <string>
#include <iostream>
#include <vector>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include <nlohmann/json.hpp>


int main() {
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t subscriber{context, zmq::socket_type::sub};
    subscriber.connect("tcp://localhost:5555");

    const std::string filter = "{";
    subscriber.set(zmq::sockopt::subscribe, filter);

    std::vector<zmq::message_t> msgs;

    cv::Mat currentImage;

    for (auto request_num = 0; request_num < 10; ++request_num) {
        zmq::recv_multipart(subscriber, std::back_inserter(msgs));

        const zmq::message_t &metadataMsg = msgs.front();
        const zmq::message_t &imageMsg = msgs.back();


        nlohmann::json metadata = nlohmann::json::parse(metadataMsg.to_string());
        const int imgType = metadata.at("type");
        const int imgWidth = metadata.at("width");
        const int imgHeight = metadata.at("height");

        std::cout << "Received: " << std::endl << metadata << std::endl;
        void *dataPointer = (void *) imageMsg.data<uchar>();

        currentImage = cv::Mat(
                imgWidth,
                imgHeight,
                imgType,
                dataPointer
        );

        msgs.clear();

        cv::imshow("Image", currentImage);
        cv::waitKey(1000);
    }

    return 0;
}

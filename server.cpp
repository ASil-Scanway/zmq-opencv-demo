#include <string>
#include <chrono>
#include <thread>
#include <vector>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <boost/filesystem.hpp>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include <nlohmann/json.hpp>

namespace fs = boost::filesystem;
using namespace std::chrono_literals;


//argv[1] - path to tif image
int main(int argc, char *argv[]) {
    fs::path imgPath = std::string(argv[1]);

    cv::Mat img = cv::imread(imgPath.string());
    cv::Mat dsImg;

    cv::resize(img, dsImg, cv::Size(), 0.01, 0.01);


    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::pub);

    sock.bind("tcp://*:5555");

    int counter = 0;
    while (true) {
        nlohmann::json metadata = {
                {"key",       "cam::left::" + std::to_string(counter)},
                {"height",    dsImg.size().height},
                {"width",     dsImg.size().width},
                {"type",      dsImg.type()},
                {"extension", ".tif"}
        };

        std::vector<zmq::message_t> messages;
        messages.emplace_back(metadata.dump(4));

        std::vector<uchar> imageBuffer;
        cv::imencode(".tif", dsImg, imageBuffer);;
        messages.emplace_back(imageBuffer.begin(), imageBuffer.end());

        zmq::send_multipart(sock, messages);

        imageBuffer.clear();
        std::this_thread::sleep_for(1s);
        counter++;
    }
}
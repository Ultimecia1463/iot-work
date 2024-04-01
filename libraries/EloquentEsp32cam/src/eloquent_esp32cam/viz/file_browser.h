#ifndef ELOQUENT_ESP32CAM_VIZ_IMAGE_BROWSER
#define ELOQUENT_ESP32CAM_VIZ_IMAGE_BROWSER

#include <FS.h>
#include "../extra/exception.h"
#include "../extra/esp32/wifi/sta.h"
#include "../extra/esp32/http/server.h"
#include "../extra/esp32/fs/fs.h"

using namespace eloq;
using Eloquent::Extra::Exception;
using Eloquent::Extra::Esp32::Http::HttpServer;
using Eloquent::Extra::Esp32::Fs::FileSystem;

namespace Eloquent {
    namespace Esp32cam {
        namespace Viz {
            /**
             * UI for files on storage
             */
            class FileBrowser {
                public:
                    Exception exception;
                    HttpServer server;

                    /**
                     * Constructor
                     */
                    FileBrowser() :
                        exception("FileBrowser"),
                        server("FileBrowser"),
                        _fs(NULL) {

                        }

                    /**
                     * Set filesystem
                     */
                    template<typename T>
                    void fs(T& fs) {
                        _fs = &fs;
                    }

                    /**
                     * Dump self IP address
                     */
                    String address() const {
                        return String("FileBrowser is available at http://") + wifi.ip();
                    }

                    /**
                     * Start server
                     */
                    Exception& begin() {
                        if (!wifi.isConnected())
                            return exception.set("WiFi not connected");

                        if (_fs == NULL)
                            return exception.set("No filesystem set");

                        onIndex();
                        onFiles();
                        onCapture();

                        return server.beginInThread(exception);
                    }

                protected:
                    FileSystem *_fs;

                    /**
                     * Register / endpoint to get index HTML
                     */
                    void onIndex() {
                        server.onGET("/", [this](WebServer *web) {
                            static const uint8_t index[5615] = {31, 139, 8, 0, 0, 0, 0, 0, 0, 19, 213, 87, 221, 111, 219, 54, 16, 127, 207, 95, 193, 10, 193, 44, 163, 150, 228, 36, 69, 11, 24, 150, 247, 217, 2, 69, 219, 96, 232, 218, 237, 161, 43, 96, 90, 162, 34, 46, 20, 41, 144, 116, 236, 44, 240, 255, 190, 163, 68, 57, 146, 45, 201, 118, 214, 61, 140, 15, 22, 117, 188, 251, 233, 190, 121, 158, 198, 244, 14, 173, 189, 136, 9, 124, 11, 207, 24, 107, 28, 58, 111, 51, 124, 67, 126, 146, 98, 165, 136, 116, 135, 206, 236, 12, 193, 154, 26, 206, 136, 97, 165, 66, 39, 161, 140, 40, 148, 225, 181, 183, 242, 174, 214, 204, 114, 20, 92, 26, 47, 24, 169, 248, 86, 94, 178, 100, 245, 227, 146, 37, 37, 56, 110, 210, 74, 186, 220, 39, 90, 129, 10, 80, 147, 181, 246, 24, 73, 52, 202, 215, 222, 37, 202, 239, 189, 11, 103, 118, 45, 166, 129, 78, 143, 151, 117, 102, 111, 64, 127, 142, 51, 242, 20, 57, 69, 255, 238, 151, 155, 125, 251, 83, 160, 238, 248, 198, 240, 237, 121, 113, 170, 23, 34, 190, 111, 243, 44, 201, 114, 134, 53, 129, 8, 211, 36, 116, 158, 21, 241, 243, 193, 7, 55, 58, 117, 186, 148, 233, 136, 70, 121, 24, 163, 72, 48, 149, 99, 30, 58, 47, 59, 16, 170, 117, 45, 80, 153, 47, 137, 88, 242, 184, 27, 51, 208, 45, 73, 97, 79, 90, 116, 1, 170, 181, 170, 223, 224, 68, 200, 50, 97, 71, 136, 34, 202, 75, 93, 158, 106, 244, 218, 51, 217, 16, 58, 20, 61, 71, 23, 78, 149, 33, 181, 92, 236, 182, 98, 23, 194, 232, 225, 155, 36, 108, 192, 156, 140, 144, 75, 162, 245, 253, 111, 144, 148, 167, 226, 244, 71, 237, 119, 74, 86, 79, 136, 213, 81, 216, 31, 139, 226, 251, 175, 208, 127, 17, 43, 14, 237, 236, 41, 153, 118, 28, 62, 97, 68, 63, 69, 251, 211, 242, 24, 232, 251, 213, 60, 213, 137, 16, 250, 164, 222, 89, 171, 212, 87, 61, 149, 186, 219, 35, 82, 172, 174, 33, 203, 14, 212, 246, 116, 177, 212, 90, 112, 244, 67, 196, 104, 116, 107, 146, 241, 142, 112, 72, 77, 227, 255, 15, 66, 18, 103, 246, 30, 118, 40, 131, 237, 52, 40, 121, 123, 84, 232, 169, 104, 123, 222, 118, 109, 180, 245, 198, 166, 151, 128, 96, 110, 165, 217, 217, 35, 229, 153, 231, 161, 8, 231, 122, 41, 9, 202, 83, 161, 5, 242, 188, 154, 64, 237, 174, 203, 180, 119, 177, 219, 226, 118, 204, 214, 169, 20, 90, 67, 49, 190, 24, 143, 199, 25, 200, 168, 20, 190, 15, 55, 103, 133, 177, 184, 241, 212, 237, 189, 247, 114, 60, 70, 169, 184, 35, 114, 98, 9, 175, 128, 80, 92, 47, 171, 148, 130, 231, 161, 112, 95, 152, 62, 114, 9, 173, 146, 107, 111, 33, 88, 220, 226, 255, 79, 63, 190, 123, 141, 126, 125, 251, 243, 167, 207, 31, 95, 239, 216, 189, 235, 225, 105, 0, 134, 216, 235, 187, 220, 86, 15, 21, 73, 154, 107, 164, 100, 4, 161, 214, 58, 87, 147, 32, 136, 98, 238, 107, 76, 217, 138, 242, 56, 82, 202, 143, 68, 102, 218, 72, 201, 186, 35, 19, 4, 75, 158, 223, 222, 24, 158, 0, 179, 156, 114, 242, 151, 114, 80, 76, 18, 34, 247, 69, 74, 13, 12, 172, 88, 249, 245, 225, 2, 133, 40, 89, 242, 72, 83, 193, 221, 33, 122, 216, 234, 13, 61, 109, 41, 121, 141, 96, 150, 72, 18, 69, 244, 4, 141, 71, 13, 50, 163, 25, 5, 234, 213, 14, 185, 104, 243, 19, 244, 229, 107, 147, 76, 164, 20, 178, 133, 110, 211, 125, 130, 18, 204, 20, 25, 157, 53, 14, 41, 167, 186, 161, 95, 181, 116, 74, 149, 159, 16, 29, 165, 197, 100, 224, 14, 219, 57, 206, 87, 24, 88, 220, 65, 161, 234, 96, 132, 0, 43, 156, 181, 192, 109, 5, 74, 83, 193, 59, 227, 110, 158, 222, 207, 110, 154, 164, 205, 142, 65, 85, 121, 118, 27, 101, 53, 120, 30, 150, 175, 133, 230, 189, 144, 117, 117, 90, 64, 139, 99, 119, 30, 20, 97, 249, 190, 68, 15, 207, 31, 106, 223, 218, 124, 87, 124, 164, 34, 22, 47, 155, 249, 190, 105, 102, 65, 197, 17, 238, 74, 152, 41, 192, 143, 240, 240, 77, 17, 185, 195, 99, 153, 37, 205, 220, 161, 175, 114, 6, 97, 117, 254, 228, 78, 175, 32, 131, 220, 86, 221, 241, 50, 43, 18, 92, 105, 59, 228, 132, 168, 16, 232, 237, 154, 62, 176, 106, 24, 173, 13, 167, 65, 54, 79, 95, 105, 44, 181, 250, 131, 106, 72, 20, 3, 53, 25, 116, 168, 181, 69, 201, 112, 222, 132, 144, 4, 186, 103, 68, 42, 249, 17, 26, 156, 142, 81, 122, 101, 48, 58, 74, 210, 253, 98, 46, 242, 17, 50, 51, 241, 215, 34, 169, 221, 110, 47, 85, 171, 16, 57, 200, 101, 32, 15, 115, 61, 78, 63, 19, 91, 21, 66, 102, 88, 87, 115, 186, 107, 126, 250, 237, 216, 128, 157, 7, 2, 91, 246, 140, 111, 19, 217, 2, 235, 223, 132, 214, 2, 216, 216, 30, 208, 220, 118, 181, 74, 245, 54, 237, 80, 24, 134, 104, 48, 225, 166, 249, 13, 134, 246, 63, 1, 154, 65, 231, 233, 196, 46, 29, 109, 179, 253, 241, 5, 238, 2, 30, 97, 237, 22, 47, 221, 246, 21, 2, 91, 151, 150, 155, 126, 230, 71, 43, 236, 174, 149, 253, 64, 203, 107, 75, 140, 150, 146, 166, 9, 42, 206, 208, 20, 93, 192, 101, 222, 110, 134, 189, 158, 230, 231, 15, 134, 117, 131, 22, 247, 154, 168, 249, 150, 245, 16, 232, 17, 184, 37, 123, 80, 234, 224, 107, 241, 134, 174, 73, 236, 94, 14, 55, 232, 221, 162, 231, 67, 237, 0, 123, 24, 31, 106, 24, 109, 190, 178, 227, 75, 103, 27, 31, 4, 118, 116, 26, 156, 214, 158, 247, 21, 222, 227, 238, 110, 31, 198, 137, 6, 171, 173, 79, 246, 136, 153, 5, 243, 58, 170, 119, 42, 200, 36, 131, 212, 214, 46, 107, 253, 175, 187, 0, 42, 117, 12, 100, 67, 159, 224, 80, 97, 155, 101, 164, 64, 131, 82, 120, 185, 80, 112, 29, 241, 27, 247, 226, 192, 247, 106, 117, 150, 47, 85, 250, 63, 236, 179, 253, 246, 97, 70, 164, 118, 231, 6, 17, 157, 63, 24, 221, 55, 8, 199, 49, 137, 17, 204, 230, 140, 42, 221, 49, 10, 84, 203, 78, 151, 34, 135, 92, 154, 155, 137, 22, 6, 90, 162, 242, 171, 203, 8, 103, 62, 19, 17, 102, 193, 29, 252, 165, 13, 44, 118, 15, 218, 166, 243, 132, 192, 104, 120, 32, 213, 172, 25, 159, 249, 45, 135, 191, 161, 38, 205, 114, 232, 196, 224, 181, 243, 7, 216, 159, 254, 217, 221, 174, 118, 214, 220, 109, 206, 182, 195, 246, 63, 77, 16, 233, 245, 202, 19, 0, 0};
                            server.sendGzip(index, 5615);
                        });
                    }
                    
                    /**
                     * Register /files endpoint to get list of files
                     */
                    void onFiles() {
                        server.onGET("/files", [this](WebServer *web) {
                            const uint8_t limit = 40;
                            const uint16_t offset = 0;
                            uint16_t i = 0;
                            bool hasNextPage = false;
                            File root = _fs->fs()->open("/");
                            File file = root.openNextFile();    

                            // empty list
                            if (!file) {
                                root.close();
                                web->send(500, "text/plain", F("No files found"));
                                return;
                            }

                            // list files
                            while (file) {
                                String filename = file.name();

                                if (file.isDirectory()) {
                                    web->sendContent("folder:");
                                    web->sendContent(filename);
                                    web->sendContent(",");
                                    web->sendContent(String(file.size()));
                                    web->sendContent("\n");

                                    file = root.openNextFile();
                                    continue;
                                }

                                // only list jpeg files
                                //if (filename.indexOf(".jpg") < 0 && filename.indexOf(".jpeg") < 0) {
                                //    file = root.openNextFile();
                                //    continue;
                                //}

                                // seek to required position
                                if (i++ < offset) {
                                    file = root.openNextFile();
                                    continue;
                                }

                                // break after reaching page limit
                                if (i >= offset + limit) {
                                    hasNextPage = true;
                                    break;
                                }

                                web->sendContent("file:");
                                web->sendContent(filename);
                                web->sendContent(",");
                                web->sendContent(String(file.size()));
                                web->sendContent("\n");

                                file = root.openNextFile();
                            }

                            if (hasNextPage)
                                web->sendContent(":next:");

                            root.close();
                            web->sendContent("");
                        });
                    }

                    /**
                     * Capture picture on request
                     */
                    void onCapture() {
                        server.onGET("/capture", [this](WebServer *web) {
                            if (!camera.capture().isOk()) {
                                web->send(500, "text/plain", camera.exception.toString());
                                return;
                            }

                            if (!_fs->save(camera.frame).to("", "jpg").isOk()) {
                                web->send(500, "text/plain", _fs->session.exception.toString());
                                return;
                            }

                            String filename = _fs->session.lastFilename;
                            web->send(200, "text/plain", String("file:") + filename + "," + camera.frame->len);
                        });
                    }
            };
        }
    }
}

namespace eloq {
    namespace viz {
        static Eloquent::Esp32cam::Viz::FileBrowser browser;
    }
}

#endif
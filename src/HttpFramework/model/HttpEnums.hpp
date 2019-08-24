#pragma once

namespace Http {

enum class HTTPMethod { HTTP_ANY, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_PATCH, HTTP_DELETE, HTTP_OPTIONS };
enum class HTTPUploadStatus { UPLOAD_FILE_START, UPLOAD_FILE_WRITE, UPLOAD_FILE_END,
                        UPLOAD_FILE_ABORTED };
enum class HTTPClientStatus { HC_NONE, HC_WAIT_READ, HC_WAIT_CLOSE };


} /* namespace Http */


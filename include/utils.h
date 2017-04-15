/**
 * Retrieve the hostname of the HTTP request.
 */
char* get_hostname(char *http_request, char* hostname);

/**
 * Retrieve the port if exists in the HTTP request.
 * We observed that HTTPS requests put the 443 port into the hostname.
 */
char* get_port(char *http_request);

/**
  Check if the given HTTP request is GET method
 */
int is_GET(char *http_request);

/**
 * A HTTP request send by a web broswer contains a lot of information (cookie, cache ...)
 * This method create the simple HTTP request as possible.
 * Currently, we are just interested by GET and Host values and Connection (always close).
 */
void create_new_http_request(char *old_http_request, char *new_request);

/*
 * io_service_post.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: rpartridge
 */
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

class Response :
    public boost::enable_shared_from_this<Response>,
    private boost::noncopyable
{
public:
    explicit Response(int result) :
        result_(result)
    {
    	std::cout << "creating response (default)" << std::endl;
    }

    virtual ~Response()
    {
    	std::cout << "destroying response" << std::endl;
    }

    void processResponse()
    {
        std::cout << "Return code: " << result_ << std::endl;
    }

private:
    int result_;
};

class Request :
    public boost::enable_shared_from_this<Request>,
    private boost::noncopyable
{
public:
    Request(boost::asio::io_service& requestService, boost::asio::io_service& responseService, const std::string& message) :
        requestService_(requestService),
        responseService_(responseService),
        message_(message)
    {
        std::cout << "creating request (default)" << std::endl;
        signal_.reset();
    }

    virtual ~Request()
    {
    	std::cout << "destroying request" << std::endl;
        if (signal_)
        {
            signal_->cancel();
        }
        signal_.reset();
    }

    void processRequest()
    {
        std::cout << "Incoming request message: " << message_ << std::endl;
        signal_.reset();

        std::cout << "Setting the signal set" << std::endl;
        signal_ = boost::shared_ptr<boost::asio::signal_set>(new boost::asio::signal_set(requestService_, SIGCHLD));

        std::cout << "Informing the io_services of the pending fork" << std::endl;
        requestService_.notify_fork(boost::asio::io_service::fork_prepare);
        responseService_.notify_fork(boost::asio::io_service::fork_prepare);

        pid_t pid = fork();
        if (pid == 0)
        {
            // this is the child process
            std::cout << "[child] Informing the child io_services of the completed fork" << std::endl;
            requestService_.notify_fork(boost::asio::io_service::fork_child);
            responseService_.notify_fork(boost::asio::io_service::fork_child);

            std::cout << "[child] Canceling the signal on the child" << std::endl;
            signal_->cancel();

            std::cout << "[child] Exec-ing the new process" << std::endl;
            const char* cmd = "/bin/false";
            execl(cmd, cmd, NULL);

            // the execl call only returns if an error has occurred,
            // so exit immediately
            exit(-1);
        }
        else
        {
            // we are the parent (or there was an error in fork)
            std::cout << "[parent] Informing the parent io_services of the completed fork" << std::endl;
            requestService_.notify_fork(boost::asio::io_service::fork_parent);
            responseService_.notify_fork(boost::asio::io_service::fork_parent);

            if (pid > 0)
            {
                std::cout << "[parent] Capturing the child pid (" << pid << ")" << std::endl;
                childPid_ = pid;

                startSignalWait();
            }
        }
    }

    void handleSignal()
    {
        std::cout << "handling the SIGCHLD signal" << std::endl;
        int status;
        if (waitpid(childPid_, &status, 0) > 0)
        {
        	std::cout << "got a signal on the child" << std::endl;
            int exitCode = -1;
            if (WIFEXITED(status))
            {
                exitCode = WEXITSTATUS(status);
            }

            std::cout << "posting the response back" << std::endl;
            boost::shared_ptr<Response> response(boost::shared_ptr<Response>(new Response(exitCode)));
            responseService_.post(boost::bind(&Response::processResponse, response));
        }
        else
        {
        	startSignalWait();
        }
    }

    void startSignalWait()
    {
    	if (signal_)
    	{
            std::cout << "[parent] Start waiting for the child to exit" << std::endl;
    		signal_->async_wait(boost::bind(&Request::handleSignal, this->shared_from_this()));
    	}
    	else
    	{
    		std::cout << "lost the signal set" << std::endl;
    	}
    }

private:
    boost::asio::io_service& requestService_;
    boost::asio::io_service& responseService_;
    boost::shared_ptr<boost::asio::signal_set> signal_;
    std::string message_;
    pid_t childPid_;
};

int main(int argc, char* argv[])
{
    boost::asio::io_service req_io_service, resp_io_service;
    boost::asio::io_service::work req_work(req_io_service), resp_work(resp_io_service);

    std::cout << "Starting threads" << std::endl;

    boost::thread req_thread(boost::bind(&boost::asio::io_service::run, &req_io_service));
    boost::thread resp_thread(boost::bind(&boost::asio::io_service::run, &resp_io_service));

    std::cout << "Sending some work" << std::endl;
    boost::shared_ptr<Request> req(boost::shared_ptr<Request>(new Request(req_io_service, resp_io_service, "Hello world!")));
    req_io_service.post(boost::bind(&Request::processRequest, req));
    req.reset();

    req_thread.join();
    resp_thread.join();

    std::cout << "Work all done" << std::endl;
    boost::system::error_code ec;
    return 0;
}

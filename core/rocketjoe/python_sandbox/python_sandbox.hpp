#pragma once

#include <string>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>

#include <pybind11/embed.h>

#include <goblin-engineer/abstract_manager_service.hpp>
#include <goblin-engineer/components/root_manager.hpp>

#include <rocketjoe/python_sandbox/detail/forward.hpp>
#include <rocketjoe/python_sandbox/detail/jupyter/pykernel.hpp>

#include <nlohmann/json.hpp>

namespace rocketjoe { namespace services {
    namespace py = pybind11;
    using detail::jupyter::pykernel;

    enum class sandbox_mode_t : std::uint8_t {
        none = 0,
        script = 1,
        jupyter_kernel = 2,
        jupyter_engine = 3,
    };

    class BOOST_SYMBOL_VISIBLE python_sandbox_t final : public goblin_engineer::abstract_manager_service {
    public:
        python_sandbox_t(goblin_engineer::components::root_manager*, nlohmann::json&);

        ~python_sandbox_t() override = default;

        void enqueue(goblin_engineer::message, actor_zeta::execution_device*) override;

        auto start() -> void;

        auto init() -> void;

    private:

        auto jupyter_engine_init() -> void;

        auto jupyter_kernel_init() -> void;

        boost::filesystem::path jupyter_connection_path;
        boost::filesystem::path script_path;
        sandbox_mode_t mode_;
        py::scoped_interpreter python_;
        py::module pyrocketjoe;
        std::unique_ptr<python_sandbox::detail::file_manager> file_manager_;
        std::unique_ptr<python_sandbox::detail::context_manager> context_manager_;
        std::unique_ptr<zmq::context_t> zmq_context;
        zmq::socket_t heartbeat_ping_socket;
        zmq::socket_t heartbeat_pong_socket;
        std::vector<zmq::pollitem_t> jupyter_kernel_commands_polls;
        std::vector<zmq::pollitem_t> jupyter_kernel_infos_polls;
        boost::intrusive_ptr<pykernel> jupyter_kernel;
        std::unique_ptr<std::thread> commands_exuctor; ///TODO: HACK
        std::unique_ptr<std::thread> infos_exuctor;    ///TODO: HACK
    };

}} // namespace rocketjoe::services

#include <rocketjoe/python_sandbox/detail/mapreduce.hpp>

#include <deque>

#include <rocketjoe/python_sandbox/detail/context_manager.hpp>
#include <rocketjoe/python_sandbox/detail/file_manager.hpp>
#include <rocketjoe/python_sandbox/detail/context.hpp>
#include <rocketjoe/python_sandbox/detail/data_set.hpp>

namespace rocketjoe { namespace services { namespace python_sandbox { namespace detail {

            auto add_mapreduce(py::module &pyrocketjoe, context_manager *cm_) -> void  {

                auto mapreduce_submodule = pyrocketjoe.def_submodule("MapReduce");

                py::class_<context>(mapreduce_submodule, "RocketJoeContext")
                        .def(
                                py::init(
                                        [cm_](
                                              ////    std::string master
                                               /* ,*/ std::string appName
                                              ////  , std::string sparkHome
                                              ///  , std::string pyFiles
                                             ////   , std::string environment
                                              ///  , int batchSize
                                              ///  , int serializer
                                              ///  , int conf
                                             ///   , int gateway
                                             ///   , int jsc
                                            ///    , int profiler_cls
                                        ) {
                                            return cm_->create_context(appName);
                                        }
                                )
                        )
                        .def("textFile", &context::text_file);

                py::class_<data_set>(mapreduce_submodule, "DataSet")
                        .def("map", &data_set::map)
                        .def("reduceByKey", &data_set::reduce_by_key)
                        .def("flatMap", &data_set::flat_map)
                        .def("collect", &data_set::collect);

                py::class_<pipelien_data_set,data_set>(mapreduce_submodule, "PipeLineDataSet")
                        .def("map", &data_set::map)
                        .def("reduceByKey", &data_set::reduce_by_key)
                        .def("flatMap", &data_set::flat_map)
                        .def("collect", &data_set::collect);



            }

}}}}
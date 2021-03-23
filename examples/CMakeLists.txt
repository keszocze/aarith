add_executable(sqrt_example sqrt.cpp)
target_link_libraries(sqrt_example aarith::Library)

add_executable(fau_adder_evaluation fau_adder.cpp)
target_link_libraries(fau_adder_evaluation aarith::Library)

add_executable(invert_gti invert.cpp)
target_link_libraries(invert_gti aarith::Library)

add_executable(float_example float_sci_str.cpp)
target_link_libraries(float_example aarith::Library)

add_executable(simple simple.cpp)
target_link_libraries(simple aarith::Library)
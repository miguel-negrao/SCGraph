################################################################################
# build tests
################################################################################

include_directories(../)

add_definitions(-Wall -pedantic)

add_executable(test_tree test_tree ../tree)

add_executable(test_obj_loader test_obj_loader ../obj_loader ../loader_3d
../graphics ../matrix ../vector_3d ../face ../color_rgba ../material)


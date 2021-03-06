add_subdirectory(Environment)
add_subdirectory(ModuleInterfacers)
add_subdirectory(PetriLab)

set(STRAT_SOURCE
        AbstractStrategy.cpp
        IA/IAParsing.cpp)
add_library(Strategy STATIC ${STRAT_SOURCE})
target_link_libraries(Strategy ModuleInterfacers HighLevelFunctions)

include_directories(/usr/local/include/wiic/)

set(WIIMOTE_SOURCES IA/IAWiiMote.cpp wiimote/Wiimote.cpp)
add_executable(IAWiimote ${WIIMOTE_SOURCES})

add_executable(Calibration calibration/Calibration.cpp)
target_link_libraries(Calibration ModuleInterfacers HighLevelFunctions)

add_executable(ActuatorsCalibrator calibration/ActuatorsCalibrator.cpp)
target_link_libraries(ActuatorsCalibrator Strategy)

if(EXISTS "/usr/bin/petrilab")
    set(USE_PETRILAB ON)
    message(STATUS "PetriLab found at '/usr/bin/petrilab'")

    add_library(IAOneRobot IA/IAOneRobot.cpp)
    target_link_libraries(IAOneRobot Strategy PetriRuntime)

    add_executable(IAPrimary IA/IAPrimary.cpp)
    target_link_libraries(IAPrimary IAOneRobot)

    add_executable(IASecondary IA/IASecondary.cpp)
    target_link_libraries(IASecondary IAOneRobot)
else()
    set(USE_PETRILAB OFF)
endif()

add_executable(IATest IA/IATest.cpp)
target_link_libraries(IATest Strategy)

if (RASPI)
    # WiiMote #
    message(STATUS "Compiling IAWiimote for the raspi")

    target_link_libraries(IAWiimote -L${PROJECT_SOURCE_DIR}/precompiled-libraries/ wiic)

    if(USE_PETRILAB)
        target_link_libraries(IAOneRobot dl -L${PROJECT_SOURCE_DIR}/precompiled-libraries/)

        message(STATUS "Compiling PetriLab IAs for the raspi")
        execute_process(
                COMMAND petrilab --update --profile "ARM release" PetriLab/Primary.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "ARM release" PetriLab/MacroPrimaryBlueRail.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "ARM release" PetriLab/MacroPrimaryChaos.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "ARM release" PetriLab/MacroPrimaryGoldenium.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "ARM release" PetriLab/Secondary.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )
    else()
        message(WARNING "PetriLab is not installed, please run 'info/scripts/install.sh petri'")
    endif()

    # IATest
    message(STATUS "Compiling IATest for the raspi")
else()

    if(USE_PETRILAB)
        execute_process(
                COMMAND petrilab --update --profile "Default" PetriLab/Primary.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "Default" PetriLab/MacroPrimaryBlueRail.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "Default" PetriLab/MacroPrimaryChaos.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "Default" PetriLab/MacroPrimaryGoldenium.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )

        execute_process(
                COMMAND petrilab --update --profile "Default" PetriLab/Secondary.petri
                WORKING_DIRECTORY ${root_SOURCE_DIR}/src/robot/Strategy
        )
    else()
        message(WARNING "PetriLab is not installed, please run 'info/scripts/install.sh petri'")
    endif()
endif()

target_link_libraries(IAWiimote ModuleInterfacers HighLevelFunctions wiicpp)
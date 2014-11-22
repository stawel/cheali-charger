

set(CURRENT_DIR ${CORE_DIR}/menus)

set(CORE_SOURCE
EditMenu.cpp  EditName.cpp  MainMenu.h  Menu.h       Options.h            ProgramDataMenu.h  ProgramMenus.h    SettingsMenu.h  StaticMenu.h
EditMenu.h    EditName.h    Menu.cpp    Options.cpp  ProgramDataMenu.cpp  ProgramMenus.cpp   SettingsMenu.cpp  StaticMenu.cpp
)

include_directories(${CURRENT_DIR})

CHEALI_FIND("CORE_SOURCE_FILES" "${CORE_SOURCE}" "${CURRENT_DIR}" )




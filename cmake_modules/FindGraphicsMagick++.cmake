# Find graphicsmagick++

FIND_PATH(
    GRAPHICSMAGICK++_INCLUDE_DIR
    NAMES Magick++.h
    HINTS $ENV{GRAPHICSMAGICK++_DIR}/include
    PATHS /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GRAPHICSMAGICK++_LIBRARY
    NAMES graphicsMagick++ libGraphicsMagick++
    HINTS $ENV{GRAPHICSMAGICK++_DIR}/lib
    PATHS /usr/local/lib
          /usr/lib
)

SET(GRAPHICSMAGICK++_FOUND "NO")

IF( GRAPHICSMAGICK++_INCLUDE_DIR AND GRAPHICSMAGICK++_LIBRARY )
    SET(GRAPHICSMAGICK++_FOUND "YES")
ENDIF()

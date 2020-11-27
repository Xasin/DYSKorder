# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

list(APPEND srcs "main.cpp"
		"programs/bullshit.cpp"
		"programs/externals.cpp"
		"programs/lzrtag.cpp"
		"programs/utilities.cpp"
		"program/CommandChunk.cpp"
		"program/Program.cpp"
		"core/buttons.cpp"
		"core/core_adc.cpp"
		"core/core_graphics.cpp"
		"core/core.cpp"
		"core/DisplayParam.cpp"
		"core/IndicatorBulb.cpp"
		"core/segments.cpp" )

set(COMPONENT_SRCS "${srcs}")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()

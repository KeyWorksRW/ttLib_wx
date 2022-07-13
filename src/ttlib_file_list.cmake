set (ttlib_file_list
    ${CMAKE_CURRENT_LIST_DIR}/ttlib_wx.cpp       # ttlib namespace functions
    ${CMAKE_CURRENT_LIST_DIR}/ttcstr_wx.cpp      # std::string with additional methods
    ${CMAKE_CURRENT_LIST_DIR}/ttsview_wx.cpp     # std::string_view with additional methods
    ${CMAKE_CURRENT_LIST_DIR}/ttmultistr_wx.cpp  # Breaks a single string into multiple strings or views
    ${CMAKE_CURRENT_LIST_DIR}/tttextfile_wx.cpp  # Classes for reading and writing text files
    ${CMAKE_CURRENT_LIST_DIR}/ttcvector_wx.cpp   # Vector class for storing ttlib::cstr strings
    ${CMAKE_CURRENT_LIST_DIR}/ttparser_wx.cpp    # Command line parser
    ${CMAKE_CURRENT_LIST_DIR}/ttstring_wx.cpp    # Enhanced version of wxString
)

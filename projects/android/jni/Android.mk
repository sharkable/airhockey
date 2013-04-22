LOCAL_PATH := ../../src/airhockey/android/jni

include $(CLEAR_VARS)

LOCAL_MODULE := airhockey

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL \
                -include ../../ios/AirHockey_Prefix.pch

LOCAL_CPPFLAGS += -std=gnu++0x -frtti

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../.. $(LOCAL_PATH)/../../../../gameengine/src 

LOCAL_CPP_EXTENSION := .cc .cpp

LOCAL_SRC_FILES := \
    app-android.c \
    airhockey.cc \
    ../../../../gameengine/src/gameengine/android/ad_engine_android.cc \
    ../../../../gameengine/src/gameengine/android/analytics_engine_android.cc \
    ../../../../gameengine/src/gameengine/android/asset_reader_android.cc \
    ../../../../gameengine/src/gameengine/android/game_engine_factory_android.cc \
    ../../../../gameengine/src/gameengine/android/local_store.cc \
    ../../../../gameengine/src/gameengine/android/resource_loader.cc \
    ../../../../gameengine/src/gameengine/animation.cc \
    ../../../../gameengine/src/gameengine/engine_view.cc \
    ../../../../gameengine/src/gameengine/entities/animatable.cc \
    ../../../../gameengine/src/gameengine/entities/button.cc \
    ../../../../gameengine/src/gameengine/entities/multi_select.cc \
    ../../../../gameengine/src/gameengine/entities/simple_item.cc \
    ../../../../gameengine/src/gameengine/game_engine.cc \
    ../../../../gameengine/src/gameengine/opengl/texture2d.cc \
    ../../../../gameengine/src/gameengine/positions.cc \
    ../../../../gameengine/src/gameengine/resource_loader.cc \
    ../../../../gameengine/src/soundengine/android/SoundPlayer.cc \
    ../../entities/paddle.cc \
    ../../entities/post.cc \
    ../../entities/puck.cc \
    ../../entities/rink.cc \
    ../../entities/round_thing.cc \
    ../../entities/sound_slider.cc \
    ../../views/game_menu_view.cc \
    ../../views/main_menu_view.cc \
    ../../views/play_view.cc \
    ../../views/rink_view.cc \
    ../../views/splash_view.cc \
    ../../views/story_view.cc \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Back.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Bounce.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Circ.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Cubic.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Elastic.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Expo.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Linear.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Quad.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Quart.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Quint.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/PennerEasing/Sine.cpp \
    ../../../../gameengine/src/gameengine/thirdparty/tinyxml2/tinyxml2.cpp \
    libzip/zip_add.c \
    libzip/zip_add_dir.c \
    libzip/zip_close.c \
    libzip/zip_delete.c \
    libzip/zip_dirent.c \
    libzip/zip_entry_free.c \
    libzip/zip_entry_new.c \
    libzip/zip_err_str.c \
    libzip/zip_error.c \
    libzip/zip_error_clear.c \
    libzip/zip_error_get.c \
    libzip/zip_error_get_sys_type.c \
    libzip/zip_error_strerror.c \
    libzip/zip_error_to_str.c \
    libzip/zip_fclose.c \
    libzip/zip_file_error_clear.c \
    libzip/zip_file_error_get.c \
    libzip/zip_file_get_offset.c \
    libzip/zip_file_strerror.c \
    libzip/zip_filerange_crc.c \
    libzip/zip_fopen.c \
    libzip/zip_fopen_index.c \
    libzip/zip_fread.c \
    libzip/zip_free.c \
    libzip/zip_get_archive_comment.c \
    libzip/zip_get_archive_flag.c \
    libzip/zip_get_file_comment.c \
    libzip/zip_get_num_files.c \
    libzip/zip_get_name.c \
    libzip/zip_memdup.c \
    libzip/zip_name_locate.c \
    libzip/zip_new.c \
    libzip/zip_open.c \
    libzip/zip_rename.c \
    libzip/zip_replace.c \
    libzip/zip_set_archive_comment.c \
    libzip/zip_set_archive_flag.c \
    libzip/zip_set_file_comment.c \
    libzip/zip_source_buffer.c \
    libzip/zip_source_file.c \
    libzip/zip_source_filep.c \
    libzip/zip_source_free.c \
    libzip/zip_source_function.c \
    libzip/zip_source_zip.c \
    libzip/zip_set_name.c \
    libzip/zip_stat.c \
    libzip/zip_stat_index.c \
    libzip/zip_stat_init.c \
    libzip/zip_strerror.c \
    libzip/zip_unchange.c \
    libzip/zip_unchange_all.c \
    libzip/zip_unchange_archive.c \
    libzip/zip_unchange_data.c

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_SHARED_LIBRARY)

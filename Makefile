CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.
LDFLAGS = -pthread

# Updated filenames to match your directory structure
SRCS = main.cpp \
       io/Reader.cpp \
       logger/Logger.cpp \
       output/Writer.cpp \
       processing/Processor.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = workflow_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) workflow.json
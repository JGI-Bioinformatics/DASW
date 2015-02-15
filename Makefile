

NVCCFLAGS = -O3 $(NVCCOPTIONS) -I $(TOOLKIT_INC_DIR) -I. -I $(SDK_COMMON_DIR)/inc/

CXXFLAGS = -O3 -g -fopenmp
LIBS = -lm

all: dasw directories

dasw: similarity_algorithm_cpu.o \
	smith_waterman_local.o \
	sequences.o \
	exceptions.o \
	arguments_manager.o \
	hi_res_timer.o \
	substitution_matrix.o \
	matches_manager.o \
	dasw.o 
		$(CXX) $(CXXFLAGS) $(LIBS) \
		similarity_algorithm_cpu.o \
		smith_waterman_local.o \
		sequences.o \
		exceptions.o \
		arguments_manager.o \
		substitution_matrix.o \
		matches_manager.o \
		hi_res_timer.o \
		dasw.o \
		-o dasw


#------------------------------ CPP --------------------------------------------
similarity_algorithm_cpu.o: similarity_algorithm_cpu.cpp \
  similarity_algorithm_cpu.h back_up_struct.h exceptions.h sequences.h \
  substitution_matrix.h matches_manager.h hi_res_timer.h
		$(CXX) $(CXXFLAGS)  -c ./similarity_algorithm_cpu.cpp
smith_waterman_local.o: smith_waterman_local.cpp smith_waterman_local.h \
  back_up_struct.h exceptions.h sequences.h substitution_matrix.h \
  similarity_algorithm_cpu.h matches_manager.h hi_res_timer.h
		$(CXX) $(CXXFLAGS)  -c ./smith_waterman_local.cpp
sequences.o: sequences.cpp sequences.h substitution_matrix.h exceptions.h
		$(CXX) $(CXXFLAGS)  -c ./sequences.cpp
exceptions.o: exceptions.cpp exceptions.h
		$(CXX) $(CXXFLAGS)  -c ./exceptions.cpp
sem.o: sem.cpp sem.h
		$(CXX) $(CXXFLAGS)  -c ./sem.cpp
arguments_manager.o: arguments_manager.cpp arguments_manager.h
		$(CXX) $(CXXFLAGS)  -c ./arguments_manager.cpp
substitution_matrix.o: substitution_matrix.cpp substitution_matrix.h \
  exceptions.h
		$(CXX) $(CXXFLAGS)  -c ./substitution_matrix.cpp
main.o: main.cpp sequences.h substitution_matrix.h exceptions.h \
  needleman_wunsch_global.h back_up_struct.h similarity_algorithm_cpu.h \
  matches_manager.h hi_res_timer.h needleman_wunsch_semiglobal.h \
  smith_waterman_local.h main_cu.h thread_manager.h thread.h sem.h \
  mutex.h arguments_manager.h
		$(CXX) $(CXXFLAGS)  -c ./main.cpp
thread.o: thread.cpp thread.h
		$(CXX) $(CXXFLAGS)  -c ./thread.cpp
mutex.o: mutex.cpp mutex.h
		$(CXX) $(CXXFLAGS)  -c ./mutex.cpp
matches_manager.o: matches_manager.cpp matches_manager.h exceptions.h
		$(CXX) $(CXXFLAGS)  -c ./matches_manager.cpp
needleman_wunsch_semiglobal.o: needleman_wunsch_semiglobal.cpp \
  needleman_wunsch_semiglobal.h back_up_struct.h exceptions.h sequences.h \
  substitution_matrix.h similarity_algorithm_cpu.h matches_manager.h \
  hi_res_timer.h needleman_wunsch_global.h
		$(CXX) $(CXXFLAGS)  -c ./needleman_wunsch_semiglobal.cpp
hi_res_timer.o: hi_res_timer.cpp hi_res_timer.h
		$(CXX) $(CXXFLAGS)  -c ./hi_res_timer.cpp
needleman_wunsch_global.o: needleman_wunsch_global.cpp \
  needleman_wunsch_global.h back_up_struct.h exceptions.h sequences.h \
  substitution_matrix.h similarity_algorithm_cpu.h matches_manager.h \
  hi_res_timer.h
		$(CXX) $(CXXFLAGS)  -c ./needleman_wunsch_global.cpp
thread_manager.o: thread_manager.cpp thread_manager.h thread.h \
  exceptions.h sem.h mutex.h
		$(CXX) $(CXXFLAGS)  -c ./thread_manager.cpp
dasw.o: dasw.cpp arguments_manager.h exceptions.h  matches_manager.h sequences.h similarity_algorithm_cpu.h smith_waterman_local.h substitution_matrix.h
		$(CXX) $(CXXFLAGS)  -c ./dasw.cpp

#------------------------------ DIRECTORIES ------------------------------------
directories: results 

results:
	 mkdir -p results
#-------------------------------- CLEANING -------------------------------------
clean: 
	rm -f *.o
	rm -f dasw

#--------------------------------- OTHER ---------------------------------------
ptx:
	$(NVCC) $(NVCCFLAGS) -c main_cu.cu --ptxas-options=-v 


#This variable stores the name/path of the directory with all the object files will be (relative to the make file)
OBJ_DIR=build
SRC_DIR=src
EXE_NAME=PatchworkHistBuilder

#this builds the project with no flags but those absolutely necessary
.PHONY: plain
plain: 
	@mkdir -p $(OBJ_DIR)/plain
	@cd $(OBJ_DIR)/plain; cmake -DBUILD_TYPE=Plain ../../$(SRC_DIR); make
	@mv $(OBJ_DIR)/plain/$(EXE_NAME) ./$(EXE_NAME)

#this builds the project with debug flags making it easier to attach a debugger and see what is happening
.PHONY: debug
debug: 
	@mkdir -p $(OBJ_DIR)/debug
	@cd $(OBJ_DIR)/debug; cmake -DBUILD_TYPE=Debug ../../$(SRC_DIR); make
	@mv $(OBJ_DIR)/debug/$(EXE_NAME) ./$(EXE_NAME)

#this builds the project with debug flags and with optimization flags allowing you to attach a debugger
#to see if the optimization is screwing something up
.PHONY: opt_debug
opt_debug: 
	@mkdir -p $(OBJ_DIR)/opt_debug
	@cd $(OBJ_DIR)/opt_debug; cmake -DBUILD_TYPE=Opt_Debug ../../$(SRC_DIR); make
	@mv $(OBJ_DIR)/opt_debug/$(EXE_NAME) ./$(EXE_NAME)
	
#this builds the project with no debug flags and with optimization flags making it run faster
.PHONY: release
release: 
	@mkdir -p $(OBJ_DIR)/release
	@cd $(OBJ_DIR)/release; cmake -DBUILD_TYPE=Release ../../$(SRC_DIR); make
	@mv $(OBJ_DIR)/release/$(EXE_NAME) ./$(EXE_NAME)

#this builds the project with the generic warning flags enabled to see where you might want to make changes
.PHONY: warn
warn:  
	@mkdir -p $(OBJ_DIR)/warn
	@cd $(OBJ_DIR)/warn; cmake -DBUILD_TYPE=Warn ../../$(SRC_DIR); make
	@mv $(OBJ_DIR)/warn/$(EXE_NAME) ./$(EXE_NAME)

#this builds the project with the generic warning flags enabled to see where you might want to make changes
.PHONY: warn_opt
warn_opt: 
	@mkdir -p $(OBJ_DIR)/warn_opt
	@cd $(OBJ_DIR)/warn_opt; cmake -DBUILD_TYPE=Warn_Opt ../../$(SRC_DIR); make
	@mv $(OBJ_DIR)/warn_opt/$(EXE_NAME) ./$(EXE_NAME)

.PHONY: everything
everything: debug plain opt_debug release warn warn_opt

#cleaning targets to remove various things generated by this make file
#this removes the contents of the build directories
.PHONY: clean
clean:
	-rm -rf $(OBJ_DIR)

#this runs clean and then removes the executable
.PHONY: cleanall
cleanall: clean
	-rm -f $(EXE_NAME)

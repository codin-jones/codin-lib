#
# MakeRules.inc
#

# remove the SECONDARY thing if files are getting removed 
.SECONDARY:

all: $(LIBS) $(EXECS)

libs: $(LIBS)

clean:	
	$(RM) *.o *.i *.so *.a
	$(RM) $(LIBS) $(EXECS) $(TEST_EXECS) $(OBJ_DIR)/*

tests: $(TESTS)

%.i: %.cc
	$(CXX) -E $(CPPFLAGS) $< > $@

$(OBJ_DIR):
	mkdir -p -m 777 $@


# TODO: what if files are used by both client and server then I end up with warnings like so:
#   Makefile:41: warning: overriding commands for target `objs/iFrugal_constants.d'
#   Makefile:39: warning: ignoring old commands for target `objs/iFrugal_constants.d'


#
# $1 -- source file
# -include $(dep_file)
#
define DEP_OBJ_template

$(eval src_file=${1})
$(eval dep_file=$(addprefix $(OBJ_DIR)/,$(patsubst %.cc,%.d,$(patsubst %.cpp,%.d,$(notdir ${1})))))
$(eval obj_file=$(patsubst %.d,%.o,$(dep_file)))

$(dep_file): $(src_file) | $(OBJ_DIR)
	@echo "Generating dependencies for $(src_file) > $(dep_file) ..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MT $(dep_file:.d=.o) $(src_file) | sed -e "s+ /usr/local/include/.*\\.h++g" > $(dep_file)
	@echo "Done generating dependencies for $(src_file) > $(dep_file)"

ifeq (0, $(words $(findstring $(MAKECMDGOALS), clean)))
  -include $(dep_file)
endif

$(obj_file): $(src_file) $(dep_file) | $(OBJ_DIR)
	@echo "Generating object for $(src_file) > $(obj_file) ..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $(obj_file) $(src_file)
	@echo "Done generating object for $(src_file) > $(obj_file)"

endef


#
# $1 -- lib name
# $2 -- source files
define LIB_template
$(eval target=${1})
$(eval src_list=${2})
$(eval obj_list=$(addprefix $(OBJ_DIR)/,$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(notdir $(src_list))))))

$(foreach src_file,$(src_list),$(eval $(call DEP_OBJ_template,$(src_file))))

$(target): $(obj_list)
	@echo "Creating library $(target) ..."
	$(AR) rcs $(target) $(obj_list)
	@echo "Done creating library $(target)" 
endef

# 
# $1 -- executable name
# $2 -- other dependencies
# $3 -- source files
#
define EXE_template
$(eval target=${1})
$(eval other_deps=${2})
$(eval src_list=${3})
$(eval obj_list=$(addprefix $(OBJ_DIR)/,$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(notdir $(src_list))))))

$(foreach src_file,$(src_list),$(eval $(call DEP_OBJ_template,$(src_file))))

$(target): $(other_deps) $(obj_list)
	@echo "Linking $(target) ..."
	$(CXX) -o $(target) $(obj_list) $(LDFLAGS)
	@echo "Done linking $(target)"
endef



# use -g for debugging, -w to suppress warnings
# -Wall for lots of warnings, -O3 for highly optimized code

# full debug
CFLAGS = -Wall -g -DDEBUG -I ../ -I .

# fast debug
#CFLAGS = -Wall -O3 -DDEBUG -I ../ -I .

# fully optimized
CFLAGS = -Wall -O3 -I ../ -I .
CFLAGS = -Wall -g -O0 -I ../ -I .  -Wno-reorder  -Wno-parentheses -Wno-misleading-indentation


//CCXX=g++ -fpermissive
CCXX=g++ 
CCCC=gcc
LIBS=-lglut32 -lglu32 -lopengl32 -lm -L/usr/X11R6/lib -lXi -lXmu

all: mylibs
	rm -f *.a
	ar cr libmyatom.a *.o
	ranlib libmyatom.a
#	rm -f *.o

tidy:
	rm -f *.o
	rm -f *.bak	
clean:
	rm -f *.o
	rm -f *.a
	

screen_contents_prop.o : screen_contents_prop.cpp screen_contents_prop.h  \
	../base/cheap_greps.h energy_tr.h \
	../math/distance_to_segments.h 
	rm -f screen_contents_prop.o
	$(CCXX) $(CFLAGS) -c screen_contents_prop.cpp

spatial_properties.o : spatial_properties.cpp spatial_properties.h ../base/cheap_greps.h \
	atom_lists.h pdb_atom.h energy_tr.h \
	potential_param.h potential_follow.h ../math/distance_to_segments.h \
	../math/sorted_potentials.h
	rm -f spatial_properties.o
	$(CCXX) $(CFLAGS) -c spatial_properties.cpp

#iso_energy_contour.o : iso_energy_contour.cpp iso_energy_contour.h ../base/cheap_greps.h \
#	atom_lists.h pdb_atom.h energy_tr.h \
#	potential_param.h potential_follow.h ../math/distance_to_segments.h \
#	../math/sorted_potentials.h
#	rm -f iso_energy_contour.o
#	$(CCXX) $(CFLAGS) -c iso_energy_contour.cpp
	
pot_energy_contour.o : pot_energy_contour.cpp pot_energy_contour.h ../base/cheap_greps.h \
	atom_lists.h pdb_atom.h energy_tr.h \
	../base/geom_iterator.h new_potential_follow.h ../math/distance_to_segments.h \
	../math/new_sorted_potentials.h ../math/radial_potentials.h
	rm -f pot_energy_contour.o
	$(CCXX) $(CFLAGS) -c pot_energy_contour.cpp
	
basic_pointelism.o : basic_pointelism.cpp basic_pointelism.h ../base/cheap_greps.h \
	energy_tr.h ../atom/raw_points_list.h ../data_and_config.h \
	../base/geom_iterator.h ../math/distance_to_segments.h
	rm -f basic_pointelism.o
	$(CCXX) $(CFLAGS) -c basic_pointelism.cpp
	
atom_list_modifier.o : atom_list_modifier.cpp atom_list_modifier.h \
	../chem/residue_properties.h ../math/move_around.h
	rm -f atom_list_modifier.o
	$(CCXX) $(CFLAGS) -c atom_list_modifier.cpp

# this now needs all the find_surface_molecule includes as dependent... 

atom_alignment.o : atom_alignment.cpp atom_alignment.h  ../math/move_around.h
	$(CCXX) $(CFLAGS) -c atom_alignment.cpp
	
basic_caving.o : basic_caving.cpp basic_caving.h ../base/cheap_greps.h \
	energy_tr.h ../atom/pdb_atom.h ../data_and_config.h \
	../math/various_searches.h
	rm -f basic_caving.o
	$(CCXX) $(CFLAGS) -c basic_caving.cpp
	
	
atom_display.o : atom_display.cpp atom_display.h ../base/cheap_greps.h \
	../view/text_commands.h atom_lists.h  pdb_atom.h nurbs_test.h atom_selection.h \
	../math/distance_to_segments.h  spatial_properties.h atom_alignment.h \
	../atom/basic_pointelism.h ../atom/basic_caving.h ../atom/atom_list_modifier.h
	$(CCXX) $(CFLAGS) -c atom_display.cpp
	
core_cmd_handler.o : core_cmd_handler.h core_cmd_handler.cpp atom_display.h \
	../base/cheap_greps.h  ../data_and_config.h  screen_contents_prop.h \
	../view/text_commands.h atom_lists.h  pdb_atom.h ../base/basic_symbol_store.h
	$(CCXX) $(CFLAGS) -c core_cmd_handler.cpp
	
pdb_atom.o : pdb_atom.cpp pdb_atom.h  \
	../base/points_base_classes.h ../view/mathutil.h
	$(CCXX) $(CFLAGS) -c pdb_atom.cpp
	
atom_io.o : atom_io.cpp atom_lists.h ../view/glut_commons.h ../view/glut_picking.h \
	../debug_and_support.h  pdb_reader.h own_vector.h raw_points_list.h \
        pdb_atom.h atom_list.h ../data_and_config.h connectivity_list.h 
	$(CCXX) $(CFLAGS) -c atom_io.cpp
	
atom_container.o : atom_container.cpp atom_list.h pdb_atom.h own_vector.h \
	../view/display_tree.h ../view/glut_commons.h
	$(CCXX) $(CFLAGS) -c atom_container.cpp

search_strategy.o : search_strategy.cpp search_strategy.h  atom_list.h pdb_atom.h \
	figure_param.h
	$(CCXX) $(CFLAGS) -c search_strategy.cpp
	
nurbs_test.o : search_strategy.cpp search_strategy.h  atom_list.h pdb_atom.h \
	figure_param.h
	$(CCXX) $(CFLAGS) -c nurbs_test.cpp
	
atom_list_proximity.o : atoms.h atom_list_proximity.cpp atom_list.h \
	pdb_atom.h search_strategy.h figure_param.h
	$(CCXX) $(CFLAGS) -c atom_list_proximity.cpp
	
connectivity_list.o : connectivity_list.cpp connectivity_list.h \
	../view/glut_commons.h ../view/display_tree.h molecule_view_list.h \
	atom_list.h pdb_chain_maker.h ../math/various_searches.h
	$(CCXX) $(CFLAGS) -c connectivity_list.cpp
	
mylibs :  atom_display.o pdb_atom.o atom_io.o atom_container.o nurbs_test.o \
	search_strategy.o atom_list_proximity.o connectivity_list.o core_cmd_handler.o \
	pot_energy_contour.o spatial_properties.o \
	screen_contents_prop.o basic_pointelism.o basic_caving.o \
	atom_list_modifier.o atom_alignment.o


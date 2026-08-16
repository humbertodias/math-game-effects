SUBDIRS := $(wildcard */)

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "Building in $@"
	@cd $@ && cc main.c -o main `pkg-config sdl3 --libs --cflags`

clean:
	find $(SUBDIRS) -type f -name 'main' -delete

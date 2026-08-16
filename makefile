SUBDIRS := $(patsubst %/main.c,%,$(wildcard */main.c))

.PHONY: all $(SUBDIRS) web clean

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "Building in $@"
	@cd $@ && cc main.c -o main `pkg-config sdl3 --libs --cflags`

web:
	python3 web/build.py

clean:
	find $(SUBDIRS) -type f -name 'main' -delete
	rm -rf dist

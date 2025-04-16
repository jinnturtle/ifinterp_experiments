PUML_SRC_DIR = puml_src
PUML_SRC =\
	parser.puml \
	things.puml \

PUML_OUT_DIR = $(PWD)/graphs
PUML_OUT = $(PUML_SRC:%.puml=$(PUML_OUT_DIR)/%.svg)

all: $(PUML_OUT) $(PUML_OUT_DIR)

$(PUML_OUT_DIR)/%.svg: $(PUML_SRC_DIR)/%.puml
	plantuml -tsvg -o $(PUML_OUT_DIR) $<

$(PUML_OUT_DIR):
	mkdir $@

.PHONY: clean
clean:
	rm -fv $(PUML_OUT)
	rmdir $(PUML_OUT_DIR)

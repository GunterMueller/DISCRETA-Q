all: ../../bin/discretaq \
	../../bin/discretaq_blockingsets \
	../../bin/discretaq_codes \
	../../bin/discretaq_minihyper_parameters \
	../../bin/discretaq_griesmer_bound \
	../../bin/discretaq_min_dist \

../../bin/discretaq:  discretaq.C
	g++  discretaq.C -o ../../bin/discretaq -O4

../../bin/discretaq_blockingsets:  discretaq_blockingsets.C
	g++  discretaq_blockingsets.C -o ../../bin/discretaq_blockingsets -O4

../../bin/discretaq_codes:  discretaq_codes.C
	g++  discretaq_codes.C -o ../../bin/discretaq_codes -O4

../../bin/discretaq_minihyper_parameters:  discretaq_minihyper_parameters.C
	g++  discretaq_minihyper_parameters.C -o ../../bin/discretaq_minihyper_parameters

../../bin/discretaq_griesmer_bound:  discretaq_griesmer_bound.C
	g++  discretaq_griesmer_bound.C -o ../../bin/discretaq_griesmer_bound

../../bin/discretaq_min_dist:  discretaq_min_dist.C
	g++  discretaq_min_dist.C -o ../../bin/discretaq_min_dist -O4

clean:
	rm -f ../../bin/discretaq \
	../../bin/discretaq_blockingsets \
	../../bin/discretaq_codes \
	../../bin/discretaq_minihyper_parameters \
	../../bin/discretaq_griesmer_bound \
	../../bin/discretaq_min_dist \






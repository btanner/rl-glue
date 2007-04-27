all:
	make -f makeGlue clean
	make -f makeGlue AGENT_ENV_BENCHMARK_SEPARATE

	make -f makeAgent clean
	make -f makeAgent

	make -f makeEnvironment clean
	make -f makeEnvironment

	make -f makeBenchmark clean
	make -f makeBenchmark

g:
	make -f makeGlue clean
	make -f makeGlue COMPILE_TOGETHER

a:
	make -f makeGlue clean
	make -f makeGlue AGENT_SEPARATE

	make -f makeAgent clean
	make -f makeAgent

e:
	make -f makeGlue clean
	make -f makeGlue ENV_SEPARATE

	make -f makeEnvironment clean
	make -f makeEnvironment

b:
	make -f makeGlue clean
	make -f makeGlue BENCHMARK_SEPARATE

	make -f makeBenchmark clean
	make -f makeBenchmark

clean:
	make -f makeGlue clean
	make -f makeAgent clean
	make -f makeEnvironment clean
	make -f makeBenchmark clean

tidy:
	make -f makeGlue tidy
	make -f makeAgent tidy
	make -f makeEnvironment tidy
	make -f makeBenchmark tidy

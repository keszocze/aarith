
branch_config() {
	if [ "$1" != "development" ]; then
		cat <<ANB
sonar.branch.name=$(echo $*)
	sonar.branch.target=development
ANB
	fi
}

cat <<EOF
	sonar.projectKey=aarith

	sonar.projectName=Aarith
	sonar.projectVersion=0.2

	sonar.sources=./src,./tests,./benchmarks
	
	sonar.cfamily.build-wrapper-output=./build/gcc/bw_output
	sonar.cfamily.threads=$(nproc)

	sonar.cfamily.cache.enabled=false

	$(branch_config $1)

	sonar.sourceEncoding=UTF-8
	# for some reason, I can't get the paths to work fine
	# so I am disabling this
#	sonar.coverageReportPaths=./build/gcc/tests/coverage/coverage.xml

	sonar.host.url=https://codesignp211.informatik.uni-erlangen.de/sonarqube
EOF

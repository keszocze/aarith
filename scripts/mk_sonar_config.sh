#!/bin/bash

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
	sonar.projectVersion=0.1

	sonar.sources=./src,./tests
	
	sonar.cfamily.build-wrapper-output=./build/bw_output
	sonar.cfamily.threads=$(nproc)

	$(branch_config $1)

	sonar.sourceEncoding=UTF-8
	sonar.coverageReportPaths=./build/gcc/tests/coverage/coverage.xml

	sonar.host.url=https://codesignp211.informatik.uni-erlangen.de/sonarqube
EOF

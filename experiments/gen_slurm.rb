#!/bin/ruby
#

outpath="/slurmdata/output/keszocze"
errorpath="/slurmdata/output/keszocze"
mail="oliver.keszoecze@fau.de"


["integer8","integer16", 
 "uinteger32-add", "uinteger32-sub", "uinteger32-mul",
 "uinteger32-karazuba", "uinteger32-div", "uinteger32-mod",
 "integer32-add", "integer32-sub", "integer32-mul",
 "integer32-div", "integer32-mod"
].each { |t|

  filename="#{t}_test.sh"

content=<<-SLURMFILE
#!/bin/bash

#SBATCH -o #{outpath}/aarith_#{t}_correctness.output.%j
#SBATCH -e #{errorpath}/aarith_#{t}_correctness.error.%j
#SBATCH --partition=Longterm
#SBATCH --mail-type=ALL
#SBATCH --mail-user=#{mail}


cd /slurmdata/output/keszocze/aarith
/slurmdata/data/keszocze/aarith/build/experiments/#{t}-correctness-experiment
SLURMFILE

puts content
puts filename

File.open(filename, 'w') { |file| file.write(content) }
}

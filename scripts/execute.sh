k=10
exec_path="../build/Vend"
data_path="../resource/orku.txt"
path_prefix="../orku/${k}"
db_path="${path_prefix}/db"
vend_prefix="${path_prefix}/encode/"
output_path_prefix="${path_prefix}/result"
output_path="${output_path_prefix}/result.csv"
pair_path_prefix="${path_prefix}/pairs/"

echo -e "complete file structure\n"
mkdir -p ${db_path} ${vend_prefix} ${pair_path_prefix} ${output_path_prefix}
echo -e "complete file structure successfully\n"

echo -e "execute\n"
${exec_path} ${data_path} ${db_path} ${vend_prefix} ${output_path} ${pair_path_prefix}
echo -e "executed successfully \n"

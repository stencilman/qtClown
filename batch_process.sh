#!/bin/sh
# lowercase any filenames with uppercase chars
#
exe_path=./win32/Release/
data_path="U:/scherbaum/unix-home/MM/morphableModel/heads/"
data_path2="H:/unix-home/data_makeup/morphableModel/newheads"
output_path="H:/unix-home/data_makeup/morphableModel/newheads"
#
for ID in `seq -f %02g 0 55`; 
	do
	for FN in 00_ls_fullOn 01_ls_gradX 02_ls_grad-X 03_ls_gradY 05_ls_gradZ 06_ls_grad-Z 10_proj_fullOn;
	do
		for i in a b c; 
			do 
				convert "U:/scherbaum/unix-home/MM/${ID}/${ID}${i}_o_${FN}.JPG" -rotate 270 out.png; 
				$exe_path/imagedeformer.exe out.png $data_path2/${ID}${i}_o_00_ls_fullOn_low_resTIMt_transform.anchors $data_path/${ID}${i}_o_00_ls_fullOn_low_resTIM.map $output_path/${ID}${i}_o_${FN}_mapped.png
				echo -n .
			done
	done
done
#	
#$data_path/01a_o_00_ls_fullOn.png $data_path/01a_o_00_ls_fullOn_low_resTIM.corrmap $data_path/01a_o_00_ls_fullOn_low_resTIM.map $data_path/put2.png


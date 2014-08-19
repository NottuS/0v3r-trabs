$(document).ready(function () {
	$.ajax({
        type: "GET",
		url: "xmls/alunos.xml",
		dataType: "xml",
		success: function(xml) {
	 		var color = {Aprovado: 'green', Matricula: 'blue', Equivale: 'yellow', 
	 							Reprovado: 'red', 'Repr. Freq': 'red'};
	 		var ras = {};
	 		var OPT;

	 		$(xml).find("MATR_ALUNO").each(function(){
	 			ras[$(this).text()] = 0;
	 		});

	 		for (var i in ras) {
	 			$("#ra").append("<option value=" + i + ">" + i + "</option>");
	 		};
	 		$(".table_grade").on('contextmenu', function(e){return false;});

	 		$("#search").click(function(){
	 			var grr = $("#ra").val();
	 			$('.codigo').unbind();
	 			$('.codigo').css("cursor", 'default');
	 			$('.codigo').css("background-color", 'white');
	 			//disc = {};
	 			OPT = {};
	 			var i = 0;
				$(xml).find("ALUNO").each(function(){
					if($(this).find("MATR_ALUNO").text() == grr){
						if($('#'+$(this).find("COD_ATIV_CURRIC").text()).length) {
							$('#'+$(this).find("COD_ATIV_CURRIC").text())
								.css("background-color", color[$(this).find("SIGLA").text()]);
								$('#'+$(this).find("COD_ATIV_CURRIC").text()).css("cursor", 'pointer');
						} else {
							if( ! ($(this).find("COD_ATIV_CURRIC").text() in OPT) ) {
								OPT[$(this).find("COD_ATIV_CURRIC").text()] = i;
								$('#'+"OPT" + i).css("background-color", color[$(this).find("SIGLA").text()]);
								$('#'+"OPT" + i).text($(this).find("COD_ATIV_CURRIC").text());
								$('#'+"OPT" + i).css("cursor", 'pointer');
								i++;
							}
						}
						
					}
				});

				getInfo = function(xml, codigo){
					var mf, freq, period, disc;

					mf = $(xml).find("MEDIA_FINAL").text();
					freq = $(xml).find("FREQUENCIA").text();
					disc = $(xml).find("NOME_ATIV_CURRIC").text();
					period = $(xml).find("PERIODO").text() + "/" + $(xml).find("ANO").text();

					return "Disciplina: " + disc + "\n" + "Código da disciplina: " + codigo + "\n" + "Cursado em: " + period
						+ "\n" + "Nota: " + mf + "\n" + "Frequência: " + freq;
				}

				var func = [];
				
				func[0] = function(codigo){
					var text;
					$(xml).find("ALUNO").each(function(){
						if($(this).find("MATR_ALUNO").text() == grr && 
								$(this).find("COD_ATIV_CURRIC").text() == codigo){
							text = getInfo(this, codigo);
						}
					});
					alert(text);
				}

				func[2] = function(codigo){
					var text = "";
					$(xml).find("ALUNO").each(function(){
						if($(this).find("MATR_ALUNO").text() == grr && 
								$(this).find("COD_ATIV_CURRIC").text() == codigo){
							text += getInfo(this, codigo) + "\n\n";
						}
					});
					alert(text);
				}

				$('.codigo').mousedown(function(event){
					if ($(this).css("background-color") != 'rgb(255, 255, 255)' && event.button != 1) {
						func[event.button]($(this).text());
					}
				});
			});
		}
	});
});
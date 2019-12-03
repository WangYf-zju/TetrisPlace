		var bStartCamera = false;
		var bShowParam = false;
		var camera_int;
		var param_int;
		$(function() {
			$('#camera-prev')[0].onerror = function() {
				clearInterval(camera_int);
				$('#camera-prev').attr('src','./res/nocmr.png');
			};
			$('#title1').click(function(){$('#camera').slideToggle(500,FinishSlideCamera);});
			$('#title2').click(function(){$('#parameter').slideToggle(500,FinishSlideParam);});
		});

		var FinishSlideCamera = function() {
			bStartCamera = !bStartCamera;
			if (bStartCamera === true) {
				camera_int = setInterval(function() {
					$('#camera-prev').attr('src','./res/cmr.jpg?time='+new Date().getTime());					
				},1000);
			} else {
				clearInterval(camera_int);
			}
		};

		var FinishSlideParam = function() {
			bShowParam = !bShowParam;
			if (bShowParam === true) {
				param_int = setInterval(function() {
					console.log(status);
					$.get('./param',function(data,status) {
						if (status == "success") {
							j = JSON.parse(data);
							$('#parameter > table').show();
							$('#parameter > div').hide();
							$('#parameter > table > tbody > tr:nth-child(2) > td:nth-child(3)').text(j.coor[0]);
							$('#parameter > table > tbody > tr:nth-child(2) > td:nth-child(5)').text(j.coor[1]);
							$('#parameter > table > tbody > tr:nth-child(2) > td:nth-child(7)').text(j.coor[2]);
							$('#parameter > table > tbody > tr:nth-child(3) > td:nth-child(3)').text(j.angle[0]);
							$('#parameter > table > tbody > tr:nth-child(3) > td:nth-child(5)').text(j.angle[1]);
							$('#parameter > table > tbody > tr:nth-child(3) > td:nth-child(7)').text(j.angle[2]);
							$('#parameter > table > tbody > tr:nth-child(4) > td:nth-child(2)').text(j.steer);
							if (j.pump == "1") {
								$('#parameter > table > tbody > tr:nth-child(5) > td:nth-child(2)').text("开");
							} else {
								$('#parameter > table > tbody > tr:nth-child(5) > td:nth-child(2)').text("关");		
							}
						} else {
							$('#parameter > table').hide();
							$('#parameter > div').show();
							clearInterval(param_int);
						}
					});						
				},1000);
		
			} else {
				clearInterval(param_int);
			}
		};
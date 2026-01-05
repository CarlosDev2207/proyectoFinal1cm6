Algoritmo SistemaBancario
	Definir opcion_principal,opcion_admin,opcion_cliente Como Entero;
	Definir acceso_correcto,continuar Como Logico;
	Definir id_cliente_1,id_cliente_2,id_cliente_3 Como Entero;
	Definir nombre_cliente_1,nombre_cliente_2,nombre_cliente_3 Como Caracter;
	Definir usuario_cliente_1,usuario_cliente_2,usuario_cliente_3 Como Caracter;
	Definir pass_cliente_1,pass_cliente_2,pass_cliente_3 Como Caracter;
	Definir num_clientes Como Entero;
	Definir num_cuenta_1,num_cuenta_2,num_cuenta_3 Como Entero;
	Definir id_dueño_1,id_dueño_2,id_dueño_3 Como Entero;
	Definir adeudo_1,adeudo_2,adeudo_3 Como Real;
	Definir limite_1,limite_2,limite_3 Como Real;
	Definir meses_inicial_1,meses_inicial_2,meses_inicial_3 Como Entero;
	Definir num_cuentas Como Entero;
	num_clientes <- 0;
	num_cuentas <- 0;
	continuar <- Verdadero;
	Repetir
		Borrar Pantalla;
		Escribir '=== MENU PRINCIPAL ===';
		Escribir '1. Administracion';
		Escribir '2. Cliente';
		Escribir '3. Salir';
		Escribir '======================';
		Escribir 'Seleccione opcion: ';
		Leer opcion_principal;
		Segun opcion_principal  Hacer
			1:
				Repetir
					Borrar Pantalla;
					Escribir '=== ADMINISTRACION ===';
					Escribir '1. Registrar cliente';
					Escribir '2. Registrar cuenta';
					Escribir '3. Ver clientes';
					Escribir '4. Ver cuentas';
					Escribir '5. Regresar al menu principal';
					Escribir '=======================';
					Escribir 'Seleccione opcion: ';
					Leer opcion_admin;
					Segun opcion_admin  Hacer
						1:
							Borrar Pantalla;
							Escribir 'REGISTRAR CLIENTE';
							Escribir '==================';
							Si num_clientes>=3 Entonces
								Escribir 'Error: Maximo 3 clientes permitidos';
							SiNo
								Escribir 'ID del cliente: ';
								Leer id_cliente_1;
								Escribir 'Nombre completo: ';
								Leer nombre_cliente_1;
								Escribir 'Usuario: ';
								Leer usuario_cliente_1;
								Escribir 'Contraseña: ';
								Leer pass_cliente_1;
								num_clientes <- num_clientes+1;
								Escribir '';
								Escribir 'Cliente registrado exitosamente';
							FinSi
							Escribir '';
							Escribir '¿Quiere regresar? (S/N)';
							Escribir 'S: Regresar al menu administracion';
							Escribir 'N: Finalizar programa';
							Definir opc_regresar Como Caracter;
							Leer opc_regresar;
							Si Mayusculas(opc_regresar)='N' Entonces
								continuar <- Falso;
								opcion_admin <- 5;
								opcion_principal <- 3;
							FinSi
						2:
							Borrar Pantalla;
							Escribir 'REGISTRAR CUENTA DE CREDITO';
							Escribir '===========================';
							Si num_cuentas>=3 Entonces
								Escribir 'Error: Maximo 3 cuentas permitidas';
							SiNo
								Definir num,id_cli,meses Como Entero;
								Definir monto_base,adeudo_ini,lim,interes,total Como Real;
								Definir cliente_existe Como Logico;
								Escribir 'Numero de cuenta: ';
								Leer num;
								Escribir 'ID del cliente dueño: ';
								Leer id_cli;
								cliente_existe <- Falso;
								Si num_clientes>0 Entonces
									Si id_cli=id_cliente_1 Entonces
										cliente_existe <- Verdadero;
									FinSi
								FinSi
								Si  NO cliente_existe Entonces
									Escribir '';
									Escribir 'Error: Cliente no existe';
								SiNo
									Escribir 'Monto base del credito: $';
									Leer monto_base;
									Escribir 'Meses para pagar inicial (3, 6 o 9): ';
									Leer meses;
									Si meses<>3 Y meses<>6 Y meses<>9 Entonces
										Escribir 'Error: Solo 3, 6 o 9 meses permitidos';
									SiNo
										interes <- meses*0.02;
										total <- monto_base*(1+interes);
										Escribir 'Limite de credito: $';
										Leer lim;
										Escribir '';
										Escribir 'Resumen del credito:';
										Escribir 'Monto base: $',monto_base;
										Escribir 'Meses: ',meses;
										Escribir 'Interes (',meses,' meses al 2%): ',interes*100,'%';
										Escribir 'Adeudo inicial total: $',total;
										Escribir 'Limite de credito: $',lim;
										Si num_cuentas=0 Entonces
											num_cuenta_1 <- num;
											id_dueño_1 <- id_cli;
											adeudo_1 <- total;
											limite_1 <- lim;
											meses_inicial_1 <- meses;
										SiNo
											Si num_cuentas=1 Entonces
												num_cuenta_2 <- num;
												id_dueño_2 <- id_cli;
												adeudo_2 <- total;
												limite_2 <- lim;
												meses_inicial_2 <- meses;
											SiNo
												num_cuenta_3 <- num;
												id_dueño_3 <- id_cli;
												adeudo_3 <- total;
												limite_3 <- lim;
												meses_inicial_3 <- meses;
											FinSi
										FinSi
										num_cuentas <- num_cuentas+1;
										Escribir '';
										Escribir 'Cuenta de credito registrada exitosamente';
									FinSi
								FinSi
							FinSi
							Escribir '';
							Escribir '¿Quiere regresar? (S/N)';
							Escribir 'S: Regresar al menu administracion';
							Escribir 'N: Finalizar programa';
							Definir opc_regresar2 Como Caracter;
							Leer opc_regresar2;
							Si Mayusculas(opc_regresar2)='N' Entonces
								continuar <- Falso;
								opcion_admin <- 5;
								opcion_principal <- 3;
							FinSi
						3:
							Borrar Pantalla;
							Escribir 'LISTA DE CLIENTES';
							Escribir '==================';
							Si num_clientes=0 Entonces
								Escribir 'No hay clientes registrados';
							SiNo
								Si num_clientes>=1 Entonces
									Escribir 'ID: ',id_cliente_1,' | Nombre: ',nombre_cliente_1,' | Usuario: ',usuario_cliente_1;
								FinSi
								Si num_clientes>=2 Entonces
									Escribir 'ID: ',id_cliente_2,' | Nombre: ',nombre_cliente_2,' | Usuario: ',usuario_cliente_2;
								FinSi
								Si num_clientes>=3 Entonces
									Escribir 'ID: ',id_cliente_3,' | Nombre: ',nombre_cliente_3,' | Usuario: ',usuario_cliente_3;
								FinSi
							FinSi
							Escribir '';
							Escribir 'Generar archivo de texto? (S/N): ';
							Definir opc Como Caracter;
							Leer opc;
							Si Mayusculas(opc)='S' Entonces
								Escribir 'Archivo generado exitosamente';
							FinSi
							Escribir '';
							Escribir '¿Quiere regresar? (S/N)';
							Escribir 'S: Regresar al menu administracion';
							Escribir 'N: Finalizar programa';
							Definir opc_regresar3 Como Caracter;
							Leer opc_regresar3;
							Si Mayusculas(opc_regresar3)='N' Entonces
								continuar <- Falso;
								opcion_admin <- 5;
								opcion_principal <- 3;
							FinSi
						4:
							Borrar Pantalla;
							Escribir 'LISTA DE CUENTAS DE CREDITO';
							Escribir '===========================';
							Si num_cuentas=0 Entonces
								Escribir 'No hay cuentas registradas';
							SiNo
								Escribir 'Cuenta | Cliente ID | Adeudo | Limite | Meses Inicial';
								Escribir '--------------------------------------------------------';
								Si num_cuentas>=1 Entonces
									Escribir num_cuenta_1,' | ',id_dueño_1,' | $',adeudo_1,' | $',limite_1,' | ',meses_inicial_1,' meses';
								FinSi
								Si num_cuentas>=2 Entonces
									Escribir num_cuenta_2,' | ',id_dueño_2,' | $',adeudo_2,' | $',limite_2,' | ',meses_inicial_2,' meses';
								FinSi
								Si num_cuentas>=3 Entonces
									Escribir num_cuenta_3,' | ',id_dueño_3,' | $',adeudo_3,' | $',limite_3,' | ',meses_inicial_3,' meses';
								FinSi
							FinSi
							Escribir '';
							Escribir 'Generar archivo de texto? (S/N): ';
							Definir opc2 Como Caracter;
							Leer opc2;
							Si Mayusculas(opc2)='S' Entonces
								Escribir 'Archivo generado exitosamente';
							FinSi
							Escribir '';
							Escribir '¿Quiere regresar? (S/N)';
							Escribir 'S: Regresar al menu administracion';
							Escribir 'N: Finalizar programa';
							Definir opc_regresar4 Como Caracter;
							Leer opc_regresar4;
							Si Mayusculas(opc_regresar4)='N' Entonces
								continuar <- Falso;
								opcion_admin <- 5;
								opcion_principal <- 3;
							FinSi
						5:
						De Otro Modo:
							Escribir 'Opcion invalida. Intente nuevamente.';
							Escribir '';
							Escribir '¿Quiere regresar? (S/N)';
							Escribir 'S: Regresar al menu administracion';
							Escribir 'N: Finalizar programa';
							Definir opc_regresar5 Como Caracter;
							Leer opc_regresar5;
							Si Mayusculas(opc_regresar5)='N' Entonces
								continuar <- Falso;
								opcion_admin <- 5;
								opcion_principal <- 3;
							FinSi
					FinSegun
				Hasta Que opcion_admin=5
			2:
				Borrar Pantalla;
				Escribir 'ACCESO CLIENTE';
				Escribir '==============';
				Definir cuenta_ingresada,pass_ingresada Como Caracter;
				Definir num_cuenta_buscada,id_cliente_encontrado Como Entero;
				Definir acceso_ok Como Logico;
				Escribir 'Numero de cuenta: ';
				Leer cuenta_ingresada;
				Escribir 'Contraseña: ';
				Leer pass_ingresada;
				num_cuenta_buscada <- ConvertirANumero(cuenta_ingresada);
				acceso_ok <- Falso;
				id_cliente_encontrado <- 0;
				Si num_cuentas>=1 Y num_cuenta_1=num_cuenta_buscada Entonces
					id_cliente_encontrado <- id_dueño_1;
				FinSi
				Si num_cuentas>=2 Y num_cuenta_2=num_cuenta_buscada Entonces
					id_cliente_encontrado <- id_dueño_2;
				FinSi
				Si num_cuentas>=3 Y num_cuenta_3=num_cuenta_buscada Entonces
					id_cliente_encontrado <- id_dueño_3;
				FinSi
				Si id_cliente_encontrado>0 Entonces
					Si id_cliente_encontrado=id_cliente_1 Y pass_cliente_1=pass_ingresada Entonces
						acceso_ok <- Verdadero;
					FinSi
					Si id_cliente_encontrado=id_cliente_2 Y pass_cliente_2=pass_ingresada Entonces
						acceso_ok <- Verdadero;
					FinSi
					Si id_cliente_encontrado=id_cliente_3 Y pass_cliente_3=pass_ingresada Entonces
						acceso_ok <- Verdadero;
					FinSi
				FinSi
				Si acceso_ok Entonces
					Escribir '';
					Escribir 'Acceso correcto. Bienvenido!';
					Escribir '';
					Escribir '¿Quiere continuar? (S/N)';
					Escribir 'S: Entrar al menu cliente';
					Escribir 'N: Finalizar programa';
					Definir opc_continuar Como Caracter;
					Leer opc_continuar;
					Si Mayusculas(opc_continuar)='N' Entonces
						continuar <- Falso;
						opcion_principal <- 3;
					SiNo
						Repetir
							Borrar Pantalla;
							Escribir '=== MENU CLIENTE ===';
							Escribir '1. Registrar deposito';
							Escribir '2. Registrar compra (nuevo credito)';
							Escribir '3. Ver saldo';
							Escribir '4. Regresar al menu principal';
							Escribir '======================';
							Escribir 'Seleccione opcion: ';
							Leer opcion_cliente;
							Segun opcion_cliente  Hacer
								1:
									Borrar Pantalla;
									Escribir 'REGISTRAR DEPOSITO (PAGO)';
									Escribir '========================';
									Definir monto Como Real;
									Escribir 'Monto del deposito (pago): $';
									Leer monto;
									Si monto<=0 Entonces
										Escribir '';
										Escribir 'Error: El monto debe ser positivo';
									SiNo
										Si num_cuenta_buscada=num_cuenta_1 Entonces
											adeudo_1 <- adeudo_1-monto;
											Si adeudo_1<0 Entonces
												adeudo_1 <- 0;
											FinSi
											Escribir '';
											Escribir 'Deposito (pago) realizado exitosamente';
											Escribir 'Nuevo adeudo: $',adeudo_1;
										FinSi
										Si num_cuenta_buscada=num_cuenta_2 Entonces
											adeudo_2 <- adeudo_2-monto;
											Si adeudo_2<0 Entonces
												adeudo_2 <- 0;
											FinSi
											Escribir '';
											Escribir 'Deposito (pago) realizado exitosamente';
											Escribir 'Nuevo adeudo: $',adeudo_2;
										FinSi
										Si num_cuenta_buscada=num_cuenta_3 Entonces
											adeudo_3 <- adeudo_3-monto;
											Si adeudo_3<0 Entonces
												adeudo_3 <- 0;
											FinSi
											Escribir '';
											Escribir 'Deposito (pago) realizado exitosamente';
											Escribir 'Nuevo adeudo: $',adeudo_3;
										FinSi
									FinSi
									Escribir '';
									Escribir '¿Quiere regresar? (S/N)';
									Escribir 'S: Regresar al menu cliente';
									Escribir 'N: Finalizar programa';
									Definir opc_regresar_dep Como Caracter;
									Leer opc_regresar_dep;
									Si Mayusculas(opc_regresar_dep)='N' Entonces
										continuar <- Falso;
										opcion_cliente <- 4;
										opcion_principal <- 3;
									FinSi
								2:
									Borrar Pantalla;
									Escribir 'REGISTRAR COMPRA (NUEVO CREDITO)';
									Escribir '================================';
									Definir monto_compra,interes,total Como Real;
									Definir meses Como Entero;
									Escribir 'Monto de la compra (nuevo credito): $';
									Leer monto_compra;
									Escribir 'Meses para pagar (3, 6 o 9): ';
									Leer meses;
									Si meses<>3 Y meses<>6 Y meses<>9 Entonces
										Escribir '';
										Escribir 'Error: Solo se permiten 3, 6 o 9 meses';
									SiNo
										Si monto_compra<=0 Entonces
											Escribir '';
											Escribir 'Error: El monto debe ser positivo';
										SiNo
											interes <- meses*0.02;
											total <- monto_compra*(1+interes);
											Escribir '';
											Escribir 'Resumen de la compra (nuevo credito):';
											Escribir 'Monto original: $',monto_compra;
											Escribir 'Meses seleccionados: ',meses;
											Escribir 'Interes (',meses,' meses al 2%): ',interes*100,'%';
											Escribir 'Total a pagar: $',total;
											Escribir '';
											Si num_cuenta_buscada=num_cuenta_1 Entonces
												Si (adeudo_1+total)>limite_1 Entonces
													Escribir 'Error: Excede el limite de credito';
												SiNo
													adeudo_1 <- adeudo_1+total;
													Escribir 'Compra (nuevo credito) registrada exitosamente';
													Escribir 'Nuevo adeudo total: $',adeudo_1;
												FinSi
											FinSi
											Si num_cuenta_buscada=num_cuenta_2 Entonces
												Si (adeudo_2+total)>limite_2 Entonces
													Escribir 'Error: Excede el limite de credito';
												SiNo
													adeudo_2 <- adeudo_2+total;
													Escribir 'Compra (nuevo credito) registrada exitosamente';
													Escribir 'Nuevo adeudo total: $',adeudo_2;
												FinSi
											FinSi
											Si num_cuenta_buscada=num_cuenta_3 Entonces
												Si (adeudo_3+total)>limite_3 Entonces
													Escribir 'Error: Excede el limite de credito';
												SiNo
													adeudo_3 <- adeudo_3+total;
													Escribir 'Compra (nuevo credito) registrada exitosamente';
													Escribir 'Nuevo adeudo total: $',adeudo_3;
												FinSi
											FinSi
										FinSi
									FinSi
									Escribir '';
									Escribir '¿Quiere regresar? (S/N)';
									Escribir 'S: Regresar al menu cliente';
									Escribir 'N: Finalizar programa';
									Definir opc_regresar_comp Como Caracter;
									Leer opc_regresar_comp;
									Si Mayusculas(opc_regresar_comp)='N' Entonces
										continuar <- Falso;
										opcion_cliente <- 4;
										opcion_principal <- 3;
									FinSi
								3:
									Borrar Pantalla;
									Escribir 'CONSULTA DE SALDO';
									Escribir '=================';
									Si num_cuenta_buscada=num_cuenta_1 Entonces
										Escribir 'Numero de cuenta: ',num_cuenta_1;
										Escribir 'Adeudo actual: $',adeudo_1;
										Escribir 'Limite de credito: $',limite_1;
										Escribir 'Credito disponible: $',limite_1-adeudo_1;
										Escribir 'Plazo inicial: ',meses_inicial_1,' meses';
									FinSi
									Si num_cuenta_buscada=num_cuenta_2 Entonces
										Escribir 'Numero de cuenta: ',num_cuenta_2;
										Escribir 'Adeudo actual: $',adeudo_2;
										Escribir 'Limite de credito: $',limite_2;
										Escribir 'Credito disponible: $',limite_2-adeudo_2;
										Escribir 'Plazo inicial: ',meses_inicial_2,' meses';
									FinSi
									Si num_cuenta_buscada=num_cuenta_3 Entonces
										Escribir 'Numero de cuenta: ',num_cuenta_3;
										Escribir 'Adeudo actual: $',adeudo_3;
										Escribir 'Limite de credito: $',limite_3;
										Escribir 'Credito disponible: $',limite_3-adeudo_3;
										Escribir 'Plazo inicial: ',meses_inicial_3,' meses';
									FinSi
									Escribir '';
									Escribir 'Archivo de saldo generado';
									Escribir '';
									Escribir '¿Quiere regresar? (S/N)';
									Escribir 'S: Regresar al menu cliente';
									Escribir 'N: Finalizar programa';
									Definir opc_regresar_saldo Como Caracter;
									Leer opc_regresar_saldo;
									Si Mayusculas(opc_regresar_saldo)='N' Entonces
										continuar <- Falso;
										opcion_cliente <- 4;
										opcion_principal <- 3;
									FinSi
								4:
								De Otro Modo:
									Escribir 'Opcion invalida. Intente nuevamente.';
									Escribir '';
									Escribir '¿Quiere regresar? (S/N)';
									Escribir 'S: Regresar al menu cliente';
									Escribir 'N: Finalizar programa';
									Definir opc_regresar_err Como Caracter;
									Leer opc_regresar_err;
									Si Mayusculas(opc_regresar_err)='N' Entonces
										continuar <- Falso;
										opcion_cliente <- 4;
										opcion_principal <- 3;
									FinSi
							FinSegun
						Hasta Que opcion_cliente=4
					FinSi
				SiNo
					Escribir '';
					Escribir 'Acceso denegado. Cuenta o contraseña incorrecta.';
					Escribir '';
					Escribir '¿Quiere regresar? (S/N)';
					Escribir 'S: Regresar al menu principal';
					Escribir 'N: Finalizar programa';
					Definir opc_regresar_acc Como Caracter;
					Leer opc_regresar_acc;
					Si Mayusculas(opc_regresar_acc)='N' Entonces
						continuar <- Falso;
						opcion_principal <- 3;
					FinSi
				FinSi
			3:
				Borrar Pantalla;
				Escribir '=== SISTEMA BANCARIO ===';
				Escribir '';
				Escribir '¿Seguro que desea salir? (S/N)';
				Escribir 'S: Confirmar salida';
				Escribir 'N: Regresar al menu principal';
				Definir opc_salir Como Caracter;
				Leer opc_salir;
				Si Mayusculas(opc_salir)='N' Entonces
					opcion_principal <- 0;
				SiNo
					continuar <- Falso;
				FinSi
			De Otro Modo:
				Escribir 'Opcion no valida. Intente nuevamente.';
				Escribir '';
				Escribir '¿Quiere regresar? (S/N)';
				Escribir 'S: Regresar al menu principal';
				Escribir 'N: Finalizar programa';
				Definir opc_regresar_main Como Caracter;
				Leer opc_regresar_main;
				Si Mayusculas(opc_regresar_main)='N' Entonces
					continuar <- Falso;
					opcion_principal <- 3;
				FinSi
		FinSegun
	Hasta Que opcion_principal=3 Y continuar=Falso
	Si continuar=Falso Entonces
		Borrar Pantalla;
		Escribir '=== PROGRAMA FINALIZADO ===';
		Escribir '';
		Escribir 'Gracias por usar el Sistema Bancario de ESCOM';
		Escribir 'Paga pronto!';
	FinSi
FinAlgoritmo

% Загружаем матрицы
run('matrix.m')

% K - Матрица Управляемости 
K = (ctrb(A, B));
rank_K_AB = rank(K);
[t, YbaisNum] = rref(K);
Basis_Y = sym( K(:, YbaisNum) );

% N - Матрица Наблюдаемости 
N = sym(obsv(A, C));
disp(size(N))
rank_N = rank(N);
Basis_NH = null(N);


% Управляемо но ненаблюдаемо
% Выписываем СЛАУ для описания п\п Y и NH  
% (ker X')' - Однородное СЛАУ
NH_SYS = (null(Basis_NH'))';
Y_SYS = (null(Basis_Y'))';
% Находим Базис Управляемого но НеНаблюдаемого, решая систему из 2х СЛАУ
T_Y_NH = null( [NH_SYS; Y_SYS]);

% Находим базис Управляемого и Наблюдаемого,
% {УпрНабл} = {Управл} - {Управл_НеНабл}
concated_Basis_Y = [eval(T_Y_NH), eval(Basis_Y)];
[a, b] = rref(concated_Basis_Y);
% b-номера линейнонезависимых столбцов первые индексы 1 и 2 - Упр и НеНабл
% Остальные 3 - упр и набл
T_Y_H = concated_Basis_Y(:, b(rank(T_Y_NH)+1:end));


% Находим базис НеУправл_НеНабл,
% {НеУправл_НеНабл} = {НеНабл} - {Управл_НеНабл}
concated_Basis_NH = [eval(T_Y_NH), eval(Basis_NH)];
[a, b] = rref(concated_Basis_NH);
T_NY_NH = concated_Basis_NH(:,b(rank(T_Y_NH)+1 : end));



% Найдем НеУправ_Набл вектора, которые будут ортогональными 
% другим векторам, решая СЛАУ. Ортогональные => л\н
% A = [a1; a2; ..;a_n], (a_i, b) = 0, i=1..n => Ab = 0
Sys = [T_NY_NH  T_Y_NH  T_Y_H]' ;
T_NY_H = null(Sys);

% Матрица перехода
% T
T = [T_Y_NH, T_Y_H, T_NY_NH, T_NY_H];
disp('T=');
disp(T);
% T^(-1)
inv_T = inv(T);


A1 = inv_T * A * T;
B1 = inv_T * B;
C1 = C*T;



disp('A1=');
disp(A1);
disp('B1=');
disp(B1);
disp('C1=');
disp(C1);


lambdas = sym(eig(A));
I = eye(10, 10);
for i = 1:10
    if rank( [A1 - I*lambdas(i) B1]) ~= 10
        disp(lambdas(i))
    end
end

for i = 1:10
    if rank([A1-I*lambdas(i); C1]) ~= 10
        disp(lambdas(i))
    end
end

% Проверяем на СТАБИЛИЗИРУЕМОСТЬ
% Неуправляемая часть
A_NY = A(5:10, 5:10);
lambdas_NY = eig(A_NY);
is_stab = true;
for i =1:size(lambdas_NY)
    if real( lambdas_NY(i) ) <= 0
        is_stab = false;
        break
    end
end
if (is_stab)
    disp('Система Стаблилизируема');
else
    disp('Система Нестаблилизируема');
end




% Проверка на ОБНАРУЖИВОЕМОСТЬ
A_NH = A([1, 2, 6, 7, 8], [1, 2, 6, 7, 8]);
lambdas_NH = eig(A_NH);

is_obn = true;
for i =1:size(lambdas_NH)
    if real( lambdas_NH(i) ) <= 0
        is_obn = false;
        break
    end
end
if (is_obn)
    disp('Система Обнаруживаема')
else
    disp('Система НеОбнаруживаема')
end






    
    
    
    
    
    
    
    
    
    
    
    
    


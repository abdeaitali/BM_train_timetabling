


%%% Bundles quadratic problem
% % some parameters
% u = 0.7;
% k = 10;
% % some random history of costs
% mu = rand([k 1]);
% % some random history of sub-gradients
% g = rand([k 1]);
% % some random history of objecitve function
% psi = rand([k 1]);
% % define the objective function
% H = eye(2);
% f = [-2*u*mu(end), 1];
% % define the constraints
% A = [g, -ones(k,1)];
% A = [A; -ones(k,1), zeros(k,1)];
% b = g.*mu - psi;
% b = [b; zeros(k,1)];
% % Solve the quadratic program
% x = quadprog(H,f,A,b)

%%% Quadratic objective function using symbolic expressions
% syms a
% syms b
% syms c
% syms d
% syms x1
% syms x2
% H = [a b; c d];
% X = [x1; x2];
% simplify(1/2*X.'*H*X)
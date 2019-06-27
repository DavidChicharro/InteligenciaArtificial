float Iterate (Node node, int depth, float alpha, float beta, int callingPlayer ) f
	// Leaf node
	if (depth == 0 || node.IsTerminal())
		return node.GetTotalScore(callingPlayer);
 
	// player playing: maximize
	if (node.mancalaGameState.currentPlayer == callingPlayer){
		float v = −1000;
		foreach (Node child in node.Children()){
			v = Math.Max(v, Iterate(child, depth-1, alpha, beta, callingPlayer ));
			alpha = Math.Max(alpha, v);

			if (alpha >= beta)
				break;
 			
 		}
		return v;
	}
	else{
		// opponent playing: minimize
		float v = 1000;
		foreach (Node child in node.Children()){
			v = Math.Min(v, Iterate(child, depth-1, alpha, beta, callingPlayer ));
			beta = Math.Min(beta, v);

			if (beta <= alpha)
				break;
		}
		return v;
	}
}
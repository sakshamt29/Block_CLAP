
void Block_Voting_Request(int cloud_server_leader_index, int num_p2p_nodes, block new_block, vector<commit_messages> &commitment_msg_pool)
{
    
    for(int index_cloud_server = 1; index_cloud_server <= num_p2p_nodes; ++index_cloud_server)
    {
        if(index_cloud_server != cloud_server_leader_index)
        {
            vector<transaction> transactions_pool = new_block.transactions_pool;
            bool transactions_validity_status = Validate_Transactions(transactions_pool);

            vector<signatures> ECDSA_signs = new_block.ECDSA_signs;
            bool signatures_validity_status = Validate_Signatures(ECDSA_signs);

            Merkle_root root_transactions = new_block.tree_root;
            bool MR_validity_status = Validate_MR(root_transactions);

            block_hash current_hash = new_block.current_block_hash;
            bool CBH_validity_status = Validate_BH(current_hash);

            if(transactions_validity_status && signatures_validity_status && MR_validity_status && CBH_validity_status)
            {
                Add_Commit_Message(commitment_msg_pool)
            }

        }
    }

}

void Block_CLAP_Block_Verification_Addition(vector<transaction> &transactions_pool, int transactions_threshold, int num_p2p_nodes)
{
    int cloud_server_leader_index = Cloud_Server_leader_RoundRobbin(num_p2p_nodes);
    block new_block = Create_New_Block(cloud_server_leader_index, transactions_pool, transactions_threshold);

    vector<commit_messages> commitment_msg_pool;
    Block_Voting_Request(cloud_server_leader_index, num_p2p_nodes, new_block, commitment_msg_pool);

   int minimum_approval_votes = (2*(num_p2p_nodes - 1))/3 + 1;

   if(commitment_msg_pool.size() >= minimum_approval_votes)
   {
       AddTo_Blockchain(new_block);
       Broadcast_Addition_Commit_Msgs(commitment_msg_pool);
   }

}
<?php
    $conn = new mysqli("localhost", "root", "risqkxk12", "invenControlDB");
    if($conn->connect_error) die("Connect Error: ".$conn->error);
    
    $sql = "SELECT * FROM ".$_POST["tableName"];
    if($result = $conn->query($sql)) die("Select Error: ".$conn->error);
    
    switch($_POST["tableName"]){
        case "customers":
            echo "<table>
                    <tr>
                        <td>Name</td>
                        <td>Phone</td>
                        <td>Address</td>
                        <td>Gender</td>
                    </tr>";
            if($result->num_rows > 0){
                while($data = $result->fetch_assoc()){
                    echo "<tr>
                            <td>".$data["cus_name"]."</td>
                            <td>".$data["cus_phone"]."</td>
                            <td>".$data["cus_address"]."</td>
                            <td>".$data["cus_gender"]."</td>
                          </tr>";            
                }
            }
            break;
        case "transactions":
            echo "<table>
                    <tr>
                        <td>Transaction Number</td>
                        <td>Product ID</td>
                        <td>Price</td>
                        <td>Date</td>
                        <td>Customer Name</td>
                    </tr>";
            if($result->num_rows > 0){
                while($data = $result->fetch_assoc()){
                    echo "<tr>
                            <td>".$data["trans_transactionNumber"]."</td>
                            <td>".$data["trans_productID"]."</td>
                            <td>".$data["trans_price"]."</td>
                            <td>".$data["trans_date"]."</td>
                            <td>".$data["trans_customerName"]."</td>
                          </tr>";          
                }
            }
            break;
        case "products":
            echo "<table>
                    <tr>
                        <td>Name</td>
                        <td>Product ID</td>
                        <td>Supplier Name</td>
                    </tr>";
            if($result->num_rows > 0){
                while($data = $result->fetch_assoc()){
                    echo "<tr>
                            <td>".$data["pro_name"]."</td>
                            <td>".$data["pro_productID"]."</td>
                            <td>".$data["pro_supplierName"]."</td>
                          </tr>";        
                }
            }
            break;
    }

    if($result->num_rows > 0){
        while($data = $result->fetch_assoc()){
            
        }
    }
    echo "</table>";
?>
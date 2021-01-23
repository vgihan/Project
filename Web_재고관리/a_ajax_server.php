<?php
    $conn = new mysqli("localhost:3306", "root", "254425", "invenControlDB");
    if($conn->connect_error) die("connect error : ".$conn->error);

    $sql = "SELECT pro_name, COUNT(IF(cus_gender = 'Male', 1, NULL)) AS male, COUNT(IF(cus_gender = 'Female', 1, NULL)) AS female 
            FROM products pro, transactions trans, customers cus
            WHERE trans.trans_productID = pro.pro_productID AND trans.trans_customerName = cus.cus_name
            GROUP BY pro_productID
            HAVING male > female;";
    if($conn->query($sql) == FALSE) die("select error : ".$conn->error);

    echo "<table>
            <tr>
                <td>Product Name</td>
            </tr>";

    $result = $conn->query($sql);
    if($result->num_rows > 0){
        while($data = $result->fetch_assoc()){
            echo "<tr>
                    <td>".$data["pro_name"]."</td>
                  </tr>";
        }
    }
    echo "</table>";
?>
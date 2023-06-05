import { Component, NgModule, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { AlertController, IonicModule, IonicSafeString } from '@ionic/angular';
import { CodeService } from '../code.service';

@Component({
  selector: 'app-t1',
  templateUrl: './t1.page.html',
  styleUrls: ['./t1.page.scss'],
  standalone: true,
  imports: [IonicModule, CommonModule, FormsModule]
})
  
 
export class T1Page implements OnInit {
  public alertButtons = ['OK'];

  currentInsertedCode = ""
  keyboardSelected = 2;
  constructor(private codeService: CodeService, public alertController: AlertController) {
  }

  ngOnInit() {
  }

  setKeyoboard(e: Event) {
    //@ts-ignore
    this.keyboardSelected = e.detail.value;
  }

  setNumber(num: number) {
    this.currentInsertedCode = this.currentInsertedCode.concat(num.toString());
  }

  verifyCode() {
    console.log(this.currentInsertedCode)
    if (this.currentInsertedCode == this.codeService.expectedCode) {
      this.codeService.currentInsertedCode = this.currentInsertedCode
      this.showOk()
    } else {
      this.clearCode()
      this.showAlert()
    }
  }

  clearCode() {
    this.currentInsertedCode = ''
  }

  async showOk() {
    let msg = new IonicSafeString('<img src="../../assets/giphy.gif">');
    console.log(msg)
    let confirmationAlert = await this.alertController.create({
      header: 'Great :)',
      subHeader: 'Code asserted.',
      message: msg,
      buttons: ['OK']
    })

    await confirmationAlert.present();
  }

  showAlert() {

    this.alertController.create({
      header: 'Error :(',
      subHeader: 'Wrong pin!',
      message: 'Please type the correct value.',
      buttons: ['OK']
    }).then(res => {

      res.present();

    });

  }
}
